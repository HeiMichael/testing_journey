/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 14:24:16 by miheider          #+#    #+#             */
/*   Updated: 2025/03/05 16:21:59 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.hpp"

int non_blocking(int fd) {
    int flag = fcntl(fd, F_GETFL, 0);
    if (flag == -1) {
        perror("get flags (non_blocking) failed");
        return -1;
    }
    if (fcntl(fd, F_SETFL, flag | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL failed");
        return -1;
    }
    return 0;
}

int startServer(Configuration ConfigFile) {
    std::vector<int>    server_fd_vec;
    int                 server_fd;
    int                 epoll_fd;
    int                 client_fd;
    struct sockaddr_in  address;
    int                 addrlen = sizeof(address);
    int                 nfds;
    int                 bytes_read;
    char                buffer[BUFFER_SIZE];
    struct epoll_event  ev, events[MAX_EVENTS];
    const char *response =
        "HTTP/1.0 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 62\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<style>body{background-color: #4CAF50}</style>\r\n"
        "<h2>Jakob</h2>"; 

    //create epoll instance
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
        
    //create socket
    for(size_t i = 0; i < ConfigFile.getListenVec().size(); i++) { 
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            perror("Socket creation failed");
            exit(EXIT_FAILURE);
        }
        server_fd_vec.push_back(server_fd);
        //allow reuse of the address
        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            perror("Set socket options failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        //bind socket to port
        //memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_port = htons(ConfigFile.getListen(i));
        address.sin_addr.s_addr = INADDR_ANY;
        // std::cout << "network byte order:\r" << htons(36895) << std::endl;
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
            perror("Bind socket failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        //make the server_fd non_blocking
        if (non_blocking(server_fd) < 0) {
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        //listening
        if (listen(server_fd, 4096) < 0) {
            perror("Listen failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }
    
        //add server_fd to epoll
        memset(&ev, 0, sizeof(ev));
        ev.events = EPOLLIN;
        ev.data.fd = server_fd;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {
            perror("epoll_ctl: listen_sock");
            close (server_fd);
            close (epoll_fd);
            exit(EXIT_FAILURE);
        }
        std::cout << "Server is running on port " << htons(address.sin_port) << std::endl;
    }

    while (1) {
        nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            close(server_fd);
            close(epoll_fd);
            exit(EXIT_FAILURE);
        }

        for (int n = 0; n < nfds; ++n) {
            int fd = events[n].data.fd;
            std::vector<int>::iterator it = (std::find(server_fd_vec.begin(), server_fd_vec.end(), fd));
            if (it != server_fd_vec.end()) {
                // New client connection
                client_fd = accept(*it, (struct sockaddr *)&address, (socklen_t *)&addrlen);
                if (client_fd < 0) {
                    if (errno != EAGAIN && errno != EWOULDBLOCK) {
                        perror("accept failed");
                    }
                    continue;
                }

                std::cout << "New client connected: " << client_fd << std::endl;

                struct sockaddr_in client_address;
                socklen_t client_len = sizeof(client_address);
                if (getpeername(client_fd, (struct sockaddr *)&client_address, &client_len) == -1) {
                    perror("getpeername failed");
                } else {
                    char client_ip[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &client_address.sin_addr, client_ip, sizeof(client_ip));
                    int client_port = ntohs(client_address.sin_port);

                    std::cout << "\nClient IP: " << client_ip << std::endl;
                    std::cout << "Client Port: " << client_port << std::endl << std::endl;
                }

                if (non_blocking(client_fd) < 0) {
                    close(client_fd);
                    continue;
                }

                memset(&ev, 0, sizeof(ev));
                ev.events = EPOLLIN;
                ev.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
                    perror("epoll_ctl: conn_sock");
                    close(client_fd);
                    continue;
                }
            } else {
                // Handle client requests
                int client_fd = events[n].data.fd;
                memset(buffer, 0, sizeof(buffer));
                bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
                if (bytes_read < 0) {
                    // if (errno != EAGAIN && errno != EWOULDBLOCK) { //acc to subject: it is not allowed to check errno after read!
                        perror("Read failed");
                        close(client_fd);
                    // }
                } else if (bytes_read == 0) {
                    // Client closed the connection
                    close(client_fd);
                } else {
                    // buffer[bytes_read];
                    std::clock_t start = std::clock();
                    Request request(buffer, ConfigFile);
                    request.print();
                    std::cout << "Sending response:\n" << response << std::endl;
                    send(client_fd, response, strlen(response), 0);
                    std::clock_t end = std::clock();
                    double process_time = static_cast<double>(end - start) * 1000.0 / CLOCKS_PER_SEC;
                    std::cout << "Time to process: " << process_time << std::endl;
                    close(client_fd);
                }
            }
        }
    }
    close(epoll_fd);
    close(server_fd);
    return 0;
}

int main(int argc, char **argv) {
    //1. Parsing the configuration file
    //2. setup the server and run it
    Configuration ConfigFile;
    (void) argv;
    
    if (argc > 3) {
        std::cout << "Error:\nToo many arguments!" << std::endl;
        return 1;
    } else {
        std::string file;
        if(argv[1])
            file = argv[1];
        else
            file = "./config_files/complex.conf";
        try{   
            ConfigFile.parseConfig(file);
            std::cout << "Listensize: " << ConfigFile.getListenVec().size() << std::endl;
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
            return (1);
        }
        startServer(ConfigFile);
    }
    
    return 0;
}
