/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jseidere <jseidere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:29:18 by miheider          #+#    #+#             */
/*   Updated: 2025/03/05 14:45:35 by jseidere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fstream>
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
#include <map>
#include <stack>
#include <vector>
#include <sstream>
#include <algorithm>

# define SERVER_OPEN 0
# define SERVER_CLOSED 1

class Server;

template <typename T>
struct type {
    std::string type;
    std::map<std::string, T> map;
};

class Configuration {
    private:
        std::vector <Server>  serverVec;
        std::vector <std::string> tokens;
        std::vector <std::string> listenVec;
        int server;
    public:
        Configuration();
        ~Configuration();
        void pushServer(std::string line);
        void tokenizer(std::string line);
        void printServerList();
        void handleKeyWords(std::string line);
        void clearVec();
        void lexer();
        std::vector<Server> getServerVec() const;
        std::vector<std::string> getTokens() const;
        std::vector<std::string> getListenVec() const;
        int getListen(int index) const;
        int getServerAmount() const;
        void setServerVec(std::vector<Server> s);
        void setTokens(std::vector<std::string> t);
        int parseConfig(std::string filename);
        void checkLines(std::ifstream &file);
        void initListen();
        Server* getServer(std::string host, std::string port);
        
};
        bool isFilepath(std::string str);
        void printYellow(std::string str1, std::string str2);
        bool startsWith(const std::string& str, const std::string& prefix);
        void printVec(std::vector<std::string> vec);
        //int parseConfig();
