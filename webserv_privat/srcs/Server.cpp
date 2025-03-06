/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:11:41 by miheider          #+#    #+#             */
/*   Updated: 2025/03/06 09:39:54 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/Server.hpp"
#include "../include/Configuration.hpp"

Server::Server() {
    std::string allowedValues[] = {
        "listen", "root", "server_name", "index", "client_max_body_size", "client_header_buffer_size"
    };
    for(int i = 0; i < 6; i++) {
        this->map[allowedValues[i]] = std::vector<std::string>();
    }
}

void Location::setFilePath(std::string str) {
    filePath = str;
}

void Location::setMap(std::map <std::string, std::vector <std::string> > m) {
    map = m;
}

std::string Location::getFilePath() {
    return filePath;
}

std::vector<std::string> Server::getKey(std::string key) {
    for(std::map<std::string, std::vector <std::string> >::iterator it = map.begin(); it != map.end(); ++it) {
        std::cout << "It->first: " << it->first << " Value " << key << std::endl;
        if(it->first == key) {
                return (it->second);
        }
    }
    return (std::vector<std::string>());
}

std::map <std::string, std::vector <std::string> > Location::getMap() {
    return map;
}

void Server::setMap(std::map<std::string, std::vector <std::string> > m) {
    map = m;
}

void Server::setLocation(std::vector<Location> l) {
    loc = l;
}

std::map<std::string, std::vector <std::string> > &Server::getMap() {
    return map;
}

std::vector<Location> &Server::getLocation() {
    return loc;
}

std::string Server::getListen() {
    for (std::map<std::string, std::vector <std::string> >::iterator it = map.begin(); it != map.end(); ++it) {
        if(it->first == "listen")
            return (it->second[0]);
    }
    return ("");
}

Location::Location () {
    std::string allowedValues[] = {
        "index", "client_max_body_size", "error_page", "allowed_methods", "autoindex",
    "return", "root", "name", "upload_store", "extension", "script_path", "cgi_pass"
    };
    for(int i = 0; i < 12; i++) {
        this->map[allowedValues[i]] = std::vector<std::string>();
    }
}


void Location::handleLocationKeyWords(std::string str) {
    str = str.substr(0, str.find(" "));
    if(str == "root")
        root++;
    else if (str == "index")
        index++;
    else if (str == "autoindex")
        autoindex++;
    else if (str == "cgi_pass")
        cgiPass++;
    else if (str == "upload_store")
        uploadStore++;
}

void Location::printMapOrange() {
    std::cout << "\033[33m" << filePath << "\033[0m" << std::endl;
    for (std::map<std::string, std::vector <std::string> >::iterator it = map.begin(); it != map.end(); ++it) {
        std::cout << "\033[33m" << it->first << "\033[0m";
        std::cout << "\033[33m" << " = " << "\033[0m";
        for(std::vector <std::string>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
            std::cout << "\033[33m" << *it2 << " |"<< " " << "\033[0m";
        std::cout << std::endl;
    }
}

void Server::printLocation() {
    for(size_t i = 0; i < loc.size(); i++) {
        std::cout << "________________" << std::endl;
        loc[i].printMapOrange();
    }
}

void Server::clear() {
    std::string allowedValues[] = {
        "index", "client_max_body_size", "error_page", "allowed_methods", "autoindex",
    "return", "root", "name", "upload_store", "extension", "script_path", "cgi_pass"
    };
    for(int i = 0; i < 12; i++) {
        this->map[allowedValues[i]] = std::vector<std::string>();
    }
    map.clear();
    loc.clear();
    listen = 0;
    serverName = 0;
    root = 0;
    index = 0;
    clientMaxBodySize = 0;
    location = 0;
}

void Location::clear() {
    filePath.clear();
    map.clear();
    std::string allowedValues[] = {
        "index", "client_max_body_size", "error_page", "allowed_methods", "autoindex",
    "return", "root", "name", "upload_store", "extension", "script_path", "cgi_pass"
    };
    for(int i = 0; i < 12; i++) {
        this->map[allowedValues[i]].clear();
    }
    root = 0;
    index = 0;
    autoindex = 0;
    cgiPass = 0;
    uploadStore = 0;
}

void printVector(std::vector <std::string> vec) {
    for (size_t i = 0; i < vec.size(); i++) {
        std::cout << "Vec[" << i << "]" << vec[i] << std::endl;
    }
}

/* void Server::handleErrorPage(std::string key) {
    if
} */

void Server::pushMap(std::string key, std::string value) {
    std::vector <std::string> vec;
    int j = 0;
    std::map<std::string, std::vector<std::string> >::iterator it = map.find(key);
    if((it != map.end() && it->second.empty()) || (startsWith(key, "error_page") || startsWith(key, "return"))) {
        syntaxCheckServer(key, value);
        if(key == "client_max_body_size" || key == "client_header_buffer_size")
            value = calculateClientMaxBodySize(value);
        if(startsWith(key, "error_page") || startsWith(key, "return")) {
            key.find_first_of(" ");
            key = key.substr(key.find_first_of(" ") + 1, key.length() - 1);
        }
        if(isFilepath(value)) {
            for(size_t i = 0; i < value.length(); i++) {
                if(value[i] == '/') {
                    if(i == 0)
                        continue;
                    if(value[j] == '.')
                        vec.push_back(value.substr(j, i - j));
                    else
                        vec.push_back(value.substr(j + 1, i - j - 1));
                    j = i;
                } else if (i == value.length() - 1) {
                    vec.push_back(value.substr(j + 1, i - j));
                }
            }
        } else 
            vec.push_back(value);
        map[key] = vec;
    }
    else 
        throw std::runtime_error("Error: Invalid variable " + key + "!");
}

bool isAllowedMethod(std::string str) {
    if(str == "allowed_methods")
        return (true);
    return (false);
}

void Location::pushMap(std::string key, std::string value) {
    std::vector <std::string> vec;
    int j = 0;
    std::map<std::string, std::vector<std::string> >::iterator it = map.find(key);
    if(!it->second.empty()) 
        std::cout << key << " Not empty" << std::endl;
    if((it != map.end() && it->second.empty()) || startsWith(key, "error_page")) {
        syntaxCheckLocation(key, value);
        if(isFilepath(value)) {
            for(size_t i = 0; i < value.length(); i++) {
                if(value[i] == '/') {
                    if(i == 0)
                        continue;
                    if(value[j] == '.')
                        vec.push_back(value.substr(j, i - j));
                    else
                        vec.push_back(value.substr(j + 1, i - j - 1));
                    j = i;
                } else if (i == value.length() - 1) {
                    vec.push_back(value.substr(j + 1, i - j));
                }
            }
        } else if (isAllowedMethod(key)) { 
            for(size_t i = 0; i < value.length(); i++) {
                if(value[i] == ' ') {
                    vec.push_back(value.substr(j, i - j));
                    j = i + 1;
                }
                else if (i == value.length() - 1)
                    vec.push_back(value.substr(j, value.length()));
            }
        } else 
            vec.push_back(value);
        map[key] = vec;
    }
    else 
        throw std::runtime_error("Error: Invalid variable " + key + " " + value + "!");
    
}

bool isValidPort(std::string port) {
    for(size_t i = 0; i < port.length(); i++) {
        if(!isdigit(port[i]))
            return false;
    }
    int num = atoi(port.c_str());
    if(num < 1024 || num > 65535)
        return false;
    return true;
}

bool isKMG(char c) {
    if(c == 'M' || c == 'K' || c == 'G')
        return true;
    return false;
}

int CheckSys() {
    if(sizeof(void*) == 8)
        return (64);
    else 
        return (32);
}

std::string calculateClientMaxBodySize(std::string value) {
    unsigned long num = 0;
    std::stringstream ss;
    for(size_t i = 0; i < value.length(); i++) {
        if(isKMG(value[i]))
            num = atoi((value.substr(0, i)).c_str());
        else
            num = atoi(value.c_str());
    }
    if(value[value.length() - 1] == 'K')
        num *= 1024;
    else if(value[value.length() - 1] == 'M')
        num *= 1024 * 1024;
    else if(value[value.length() - 1] == 'G')
        num *= 1024 * 1024 * 1024;
    if(CheckSys() == 32 && num > 4294967297)
        throw std::runtime_error("Error: client_max_body_size too high!");
    else if (CheckSys() == 64 && num > 4294967297)                          // adjust number!!!
        throw std::runtime_error("Error: client_max_body_size too high!");
    ss << num;
    std::string result = ss.str();
    return (result);
}

void checkListen(std::string value) {
    if(value.empty())
        throw std::runtime_error("Error: listen can't be empty!");
    if(value.length() > 5)
        throw std::runtime_error("Error: Invalid port number: Value needs to be between 1024 & 65535!");
    if(!isValidPort(value))
        throw std::runtime_error("Error: Invalid port number: Value needs to be between 1024 & 65535 and only numbers!");
}

void checkRoot(std::string value, int flag) {
    if(value.empty() && flag == 1)
        throw std::runtime_error("Error: root can't be empty!");
    if(value.empty() && flag == 0)
        return ;
    if(!isFilepath(value))
        throw std::runtime_error("Error: Invalid path for root " + value + "!");
}

void checkServerName(std::string value) {
    if(value.empty())
        throw std::runtime_error("Error: server_name can't be empty!");
    //int dot = 0;
    for(size_t i = 0; i < value.length(); i++) {
        if(value[i] != '-' && value[i] != '.' && !isalnum(value[i]))
            throw std::runtime_error("Error: Invalid server name: " + value + "!");
        /* if(value[i] == '.')
            dot++; */
    }
    // if(dot != 1)
    //     throw std::runtime_error("Error: Invalid server name: " + value + "!");
}

void checkErrorPage(std::string key, std::string value) {
    key = key.substr(key.find(" ") + 1, key.length() - 1);
    int num = atoi(key.c_str());
    if(num < 400 || num > 599)
        throw std::runtime_error("Error: Invalid error code: " + key + "!");
    if(!isFilepath(value))
        throw std::runtime_error("Error: Invalid path for error_page!");
}

void checkClientMaxBodySize(std::string value) {
    for(size_t i = 0; i < value.length(); i++) {
        if((!isKMG(value[i]) && !isdigit(value[i])) ||
        (isKMG(value[i]) && i != value.length() - 1))
            throw std::runtime_error("Error: Invalid value for client_max_body_size!");
    }
    std::string num = calculateClientMaxBodySize(value);
}

void checkAutoindex(std::string value) {
    if(value != "on" && value != "off")
        throw std::runtime_error("Error: Invalid value for autoindex!");
}

void checkIndex(std::string value) {
    if(value.empty())
        throw std::runtime_error("Error: index can't be empty!");
    if(value.find('/') == std::string::npos && value.find('.') == std::string::npos) {
            throw std::runtime_error("Error: Invalid path for index!");
    }
}

void checkCgiPass(std::string value) {
    if(value.find(" ") != std::string::npos) {
        std::string path = value.substr(value.find(" ") + 1, value.length());
        if(!isFilepath(path))
            throw std::runtime_error("Error: Invalid path for cgi_pass!");
    }
}

void checkAllowedMethods(std::string value) {
    std::string allowedMethods[] = {"GET", "POST", "DELETE"};
    std::vector<std::string>  methods;
    int j = 0;
    for(size_t i = 0; i < value.length(); i++) {
        if(value[i] == ' ') {
            methods.push_back(value.substr(j, i - j));
            j = i + 1;
        }
        else if (i == value.length() - 1)
            methods.push_back(value.substr(j, value.length()));
    }
    for(size_t k = 0; k < methods.size(); k++) {
        //std::cout << "Method: " << methods[k] << std::endl;
        if(methods[k] != "GET" && methods[k] != "POST" && methods[k] != "DELETE")
            throw std::runtime_error("Error: Invalid method " + methods[k] + "!");
    }
    
}

void checkReturn(std::string key, std::string value) {
    std::cout << "Key: " << key << " Value: " << value << std::endl;
    if(value.empty())
        throw std::runtime_error("Error: return can't be empty");
    key = key.substr(key.find(" ") + 1, key.length() - 1);
    int num = atoi(key.c_str());
    if(num != 301)
        throw std::runtime_error("Error: return code must be 301!");
    if(!isFilepath(value))
        throw std::runtime_error("Error: Invalid path for error_page!");
}

void checkClientHeaderBufferSize(std::string value) {
    for(size_t i = 0; i < value.length(); i++) {
        if((value[i] != 'K' && !isdigit(value[i])))
            throw std::runtime_error("Error: Invalid value for client_header_buffer_size!");
    }
    std::string num = calculateClientMaxBodySize(value);
}

void syntaxCheckServer(std::string key, std::string value) {
    if(key == "listen")
        checkListen(value);
    else if (key == "root")
        checkRoot(value, 1);
    else if (key == "server_name")
        checkServerName(value);
    else if (startsWith(key, "error_page"))
        checkErrorPage(key, value);
    else if (key == "client_max_body_size")
        checkClientMaxBodySize(value);
    else if (key == "index")
        checkIndex(value);
    else if (startsWith(key, "return"))
        checkReturn(key, value);
    else if (key == "client_header_buffer_size")
        checkClientHeaderBufferSize(value);
}

void syntaxCheckLocation(std::string key, std::string value) {
    if(key == "root")
        checkRoot(value, 0);
    else if (key == "index")
        checkIndex(value);
    else if (key == "autoindex")
        checkAutoindex(value);
    else if (key == "cgi_pass")
        checkCgiPass(value);
    else if (key == "allowed_methods")
        checkAllowedMethods(value);
}
