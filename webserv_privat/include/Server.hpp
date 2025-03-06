/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jseidere <jseidere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 10:27:24 by miheider          #+#    #+#             */
/*   Updated: 2025/03/05 15:19:32 by jseidere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Configuration.hpp"
#include <map>

class Configuration;

class Location {
    private:
        std::string filePath;
        std::map <std::string, std::vector <std::string> > map;
        int root;
        int index;
        int autoindex;
        int cgiPass;
        int uploadStore;
    public:
        Location ();
        void pushMap(std::string key, std::string value);
        void printMapOrange();
        void clear();
        void handleLocationKeyWords(std::string str);
        std::string getFilePath();
        std::map <std::string, std::vector <std::string> > getMap();
        void setFilePath(std::string str);
        void setMap(std::map <std::string, std::vector <std::string> > m);
};

class Server {
    private:
       std::map<std::string, std::vector <std::string> > map;
       std::vector<Location> loc;
       int listen;
       int serverName;
       int root;
       int index;
       int clientMaxBodySize;
       int location;
    public:
        Server();
        void clear();
        void pushMap(std::string str1, std::string str2);
        void printMapPink();
        void printLocation();
        void printServerVars();
        std::map<std::string, std::vector <std::string> > &getMap();
        std::vector<Location> &getLocation();
        std::string getListen();
        void setMap(std::map<std::string, std::vector <std::string> > m);
        void setLocation(std::vector<Location> l);
        std::vector<std::string> getKey(std::string key);
};

void syntaxCheckServer(std::string key, std::string value);
void syntaxCheckLocation(std::string key, std::string value);
std::string calculateClientMaxBodySize(std::string value);

