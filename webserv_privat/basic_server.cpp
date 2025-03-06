/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_server.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jseidere <jseidere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Updated: 2025/03/03 14:18:04 by jseidere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/epoll.h>
#include <sys/socket.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <string>
#include <netinet/in.h>
#include <cstring>
#include <cstdio>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>

#define MAX_EVENTS 10
#define BUFFER_SIZE 4096 //good balance between memory usage and perfomance

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

int main() {
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
        "Content-Type: text/plain\r\n"
        "Content-Length: 62\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<style>body{background-color: #4CAF50}</style>\r\n"
        "<h2>Jakob</h2>"; 

    //create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    //allow reuse of the address
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Set socket options failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    //bind socket to port
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
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

    std::cout << "Server is running on port 8080..." << std::endl;

    //create epoll instance
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
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

    while (1) {
        nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            close(server_fd);
            close(epoll_fd);
            exit(EXIT_FAILURE);
        }

        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == server_fd) {
                // New client connection
                client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
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
                    // Client closed connection
                    close(client_fd);
                } else {
                    buffer[bytes_read];
                    std::cout << "Received request:\n" << buffer << std::endl;
                    std::cout << "Sending response:\n" << response << std::endl;
                    send(client_fd, response, strlen(response), 0);
                    close(client_fd);
                }
            }
        }
    }
    close(epoll_fd);
    close(server_fd);
    return 0;
}

