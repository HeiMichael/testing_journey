/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jseidere <jseidere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:28:58 by miheider          #+#    #+#             */
/*   Updated: 2025/03/05 14:08:14 by jseidere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Configuration.hpp"
#include "../include/Server.hpp"

//g++ -std=c++98 srcs/Configuration.cpp srcs/Server.cpp 

//rules for config file
//1. Brackets must be closed
//2. Every block declaration must be in one line
//3. after every block declaration must be a bracket
//4. after bracket variable declaration must be in one line
//5. after variable declaration must be a semicolon
//6. No consecutive semicolons
//7. No consecutive brackets
//8. No consecutive spaces
//9. Valid Filepath
//10. after listen must be number
//11. after server_name must be string
//12. after root must be string & FilePath
//13. after index must be string & FilePath
//14. after location must be string & FilePath

Configuration::Configuration() {
    server = 0;
}

Configuration::~Configuration() {
    
}

template <typename T>
void printContainerGreen(T container) {
    for (typename T::iterator it = container.begin(); it != container.end(); ++it) {
        std::cout << "\033[32m" << *it << "\033[0m " << std::endl;
    }
}


std::vector<std::string> Configuration::getTokens() const {
    return tokens;
}

std::vector<Server> Configuration::getServerVec() const {
    return serverVec;
}

int Configuration::getServerAmount() const {
    return server;
}

void Configuration::initListen() {
    for(int i = 0; i < getServerAmount(); i++) {
        if(std::find(listenVec.begin(), listenVec.end(), serverVec[i].getListen()) == listenVec.end())
            listenVec.push_back(serverVec[i].getListen());
    }
}

int Configuration::getListen(int index) const {
    int num;
    num = atoi(listenVec[index].c_str());
    return(num);
}

std::vector<std::string> Configuration::getListenVec() const{
    return(this->listenVec);
}

void Configuration::setTokens(std::vector<std::string> t) {
    tokens = t;
}

void Configuration::setServerVec(std::vector<Server> s) {
    serverVec = s;
}

bool checkBrackets(std::ifstream &file) {
    std::stack<char> stack;
    std::string line;
    while(std::getline(file, line)) {
        for(size_t i = 0; i < line.size(); i++) {
            if(line[i] == '{' || line[i] == '(' || line[i] == '[') {
                stack.push(line[i]);
            }
            if(line[i] == '}' || line[i] == ')' || line[i] == ']') {
                if(stack.empty())
                    return (false);
                if(line[i] == '}') {
                    if(stack.top() == '{')
                        stack.pop();
                    else
                        return (false);
                } 
                if(line[i] == ')') {
                    if(stack.top() == '(')
                        stack.pop();
                    else
                        return (false);
                }
                if(line[i] == ']') {
                    if(stack.top() == '[')
                        stack.pop();
                    else
                        return (false);
                }
            }
        }
    }
        if(!stack.empty())
            return (false);
        return(true);
}

void Configuration::pushServer(std::string line) {
    Server tmp;
    std::string value;
    std::string key;
    
    if(line[line.size() - 1] == ';') {
        size_t space = line.find(' ');
        key = line.substr(0, space);
        value = line.substr(space + 1, line[line.size() - 1]);
        printYellow("key", key);
        printYellow("value", value); 
        tmp.pushMap(key, value);
    }
    serverVec.push_back(tmp);
    //tmp.clear();
}

void deleteComments(std::string &line) {
    size_t pos = line.find("#");
    if(pos != std::string::npos)
        line = line.substr(0, pos);
}

void Configuration::tokenizer(std::string line) {
    size_t pos = 0;
    std::string token;
    if(line[line.size() - 1] == ';') {
        std::string trimLine = line.substr(0, line.size() - 1);
        tokens.push_back(trimLine);
        return;
    }
    while((pos = line.find(" ")) != std::string::npos) {
        if(line.length() == 0)
            continue;
        token = line.substr(0, pos);
        tokens.push_back(token);
        line.erase(0, pos + 1);
    }
    tokens.push_back(line);
}

void Configuration::clearVec() {
     for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
        if((*it).empty()) {
            it = tokens.erase(it);
            it--;
        }
     }
}

std::string trim(const std::string& str) {
    deleteComments(const_cast<std::string&>(str));
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    if(start == std::string::npos)
        return "";
    else
        return str.substr(start, end - start + 1);
}

int bracketHandler(std::string str, int &flag) {
    static int openBracket = 0;
    static int closedBracket = 0;

    if(str == "{") {
        openBracket++;
        return (true);
    }
    if(str == "}") {
        closedBracket++;
        if(closedBracket == openBracket) {
            printYellow("Closed", "");
            flag = SERVER_CLOSED;
            return (2);
        }
        return (true);
    }
    return (false);
}

bool checkSpaces(std::string line) {
    bool consecutive = false;
    for (size_t i = 0; i < line.size(); i++) {
        if(line[i] == ' ') {
            if(consecutive) {
                return false;
            }
            consecutive = true;
        } else {
            consecutive = false;
        }
    }
    return true;
}

//put line in vector and split at spaces end is semicolon
void splitLine (std::string line) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while((pos = line.find(" ")) != std::string::npos) {
        token = line.substr(0, pos);
        tokens.push_back(token);
        line.erase(0, pos + 1);
    }
    tokens.push_back(line);
    for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
        std::cout << *it << std::endl;
    }
    printContainerGreen(tokens);
}

int findSecondOccurrence(const std::string& str, char c) {
    std::size_t first = str.find(c);
    if (first == std::string::npos) {
        return -1;
    }
    std::size_t second = str.find(c, first + 1);
    if (second == std::string::npos) {
        return -1;
    }
    return second;
}

std::string to_string_c98(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

bool startsWith(const std::string& str, const std::string& prefix) {
    return str.find(prefix) == 0;
}

void Configuration::handleKeyWords(std::string line) {
    line = line.substr(0, line.find(" "));
    if(line == "server") {
        server++;
    }
}

bool checkIfBlockCommand(std::string line) {
    if(startsWith(line, "server_name"))
        return false;
    if(startsWith(line, "server") || startsWith(line, "location"))
        return true;
    return false;
}

bool checkForBrackets(std::string line) {
    if(line.find("{") != std::string::npos || line.find("}") != std::string::npos)
        return true;
    return false;
}

bool checkForSemicolon(std::string line) {
    if(line.find(";") != std::string::npos)
        return true;
    return false;
}

bool afterSemicolon(std::string line) {
    for(size_t i = 0; i < line.length(); i++) {
        if(line[i] == ';' && i != line.length() - 1)
            return (true); 
    }
    return (false);
}

void Configuration::lexer() {
    std::string key;
    std::string value;
    Location tmpLoc;
    Server tmp;

    int flag = SERVER_CLOSED;
    for (size_t i = 0; i < tokens.size(); i++) {
        if(tokens[i] == "server" && flag == SERVER_CLOSED) {
            if(i < tokens.size() && tokens[i + 1] != "{")
                throw std::runtime_error ("Error: expecting '{', but found '" + tokens[i + 1] + "' in token[" + to_string_c98(i) + "]" +"!");
            flag = SERVER_OPEN;
            tmp = Server();
            continue;
        }
        if(bracketHandler(tokens[i], flag)) {
            if(bracketHandler(tokens[i], flag) == 2) {
                serverVec.push_back(tmp);
                tmp.clear();
                continue;
            }
            else
                continue;
        }
        if(tokens[i + 1] != "{" || tokens[i - 1] == "location") {
            if(startsWith(tokens[i], "error_page") || startsWith(tokens[i], "return")) {
                key = tokens[i].substr(0, findSecondOccurrence(tokens[i], ' '));
            } else {
                key = tokens[i].substr(0, tokens[i].find(" "));
            }
            if(!checkSpaces(tokens[i])) {
                printYellow("Error: ", "Consecutive spaces in line!");
            } else if (key == "location") {
                while(tokens[i] != "}") {
                    if(tokens[i] == "location" && isFilepath(tokens[i + 1])) {
                        tmpLoc.setFilePath(tokens[i + 1]);
                        i+=2;
                    }
                    if(tokens[i] == "{")
                        i++;
                    tmpLoc.handleLocationKeyWords(tokens[i]);
                    key = tokens[i].substr(0, tokens[i].find(" "));
                    value = tokens[i].substr(tokens[i].find(" ") + 1, tokens[i].size());
                    tmpLoc.pushMap(key, value);
                    i++;
                }
                    tmp.getLocation().push_back(tmpLoc);
                    key.clear();
                    value.clear();
                    tmpLoc.clear();
            } else if (startsWith(tokens[i], "error_page") || startsWith(tokens[i], "return")) {
                value = tokens[i].substr(findSecondOccurrence(tokens[i], ' ') + 1, tokens[i].size());
            }
            else {
                value = tokens[i].substr(tokens[i].find(" ") + 1, tokens[i].size());
            }
        }
        else 
            continue;
        if (key == value) {
                value = "";
        }
        if(!key.empty()) {
            tmp.pushMap(key, value);
        }
    }
    if(flag == SERVER_OPEN)
        serverVec.push_back(tmp);
}

bool isFilepath(std::string str) {
    return (str.find("/") != std::string::npos);
}
//get through the ServerVec and check if you find host || port if valid host is found check if same
//server also has valid port if not go to other server and look for valid port
Server* Configuration::getServer(std::string host, std::string port) {
    (void) host;
    for(size_t i = 0; i < serverVec.size(); i++) {
        std::map<std::string, std::vector<std::string> >::iterator it = serverVec[i].getMap().find("server_name");
        if(it != serverVec[i].getMap().end()) {
            if(it->second[0] == host) {
               std::cout << "\033[1;34mServer Num: " << i << " has host -> " << it->second[0] << "\033[0m" << std::endl;
                    std::map<std::string, std::vector<std::string> >::iterator it2 = serverVec[i].getMap().find("listen");
                    if(it2->second[0] == port) {
                        std::cout << "\033[1;34mServer Num: " << i << " has port -> " << it2->second[0] << "\033[0m" << std::endl;               
                        return &serverVec[i];
                    }
            }
        }
    }
    for(size_t i = 0; i < serverVec.size(); i++) {
        std::map<std::string, std::vector<std::string> >::iterator it2 = serverVec[i].getMap().find("listen");
        if(it2 != serverVec[i].getMap().end()) {
            if(it2->second[0] == port){
                std::cout << "\033[1;34mServer Num: " << i << " has port -> " << it2->second[0] << "\033[0m" << std::endl;
                return &serverVec[i];
            }
        }
    }
    return (0);
}

void Configuration::checkLines(std::ifstream &file) {
    //Configuration ConfigFile;
    std::string oldLine, line;
    // int hierachy = 0;
    while (std::getline(file, line)) {
        line = trim(line);
        handleKeyWords(line);
        tokenizer(line);
        if((checkIfBlockCommand(line) || checkForBrackets(line)) && checkForSemicolon(line)) {
            throw std::runtime_error("Error: No semicolon allowed after block declaration!");
        }
        if(afterSemicolon(line))
            throw std::runtime_error("Error: Wrong Syntax!");
    }
    clearVec();
    if(getServerAmount() == 0)
        throw std::runtime_error("Error: No server blocks found!");
    printContainerGreen(getTokens());
    lexer();
    printServerList();
    //std::cout << "ServerVec size: " << ConfigFile.getServerVec().size() << std::endl;
    //ConfigFile.syntaxMap();
}

int Configuration::parseConfig(std::string filename) {
    //std::string filename = "/home/jseidere/Comon_core/Webserver/config_files/default.conf";
    //std::string filename = "/home/jseidere/Comon_core/Webserver/config_files/complex.conf";
    std::ifstream file(filename.c_str());
    if(!file.is_open()) {
        std::cerr << "Couldn't open file!" << std::endl;
        return(false);
    }
    if(!checkBrackets(file))
        std::cerr << "\033[31mCouldn't open file!\033[0m" << std::endl;
    else
        std::cout << "\033[32mBrackets closed!\033[0m" << std::endl;
    file.clear();
    file.seekg(0);
    checkLines(file);
    initListen();
    file.close();
    return (0);
}

/* int main () {
    try{   
        parseConfig();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return (1);
    }
    return (0);
} */

//delete empty dir in path
//more then 1 value in scope
//first server scope is default 
//

