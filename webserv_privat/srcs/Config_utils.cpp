#include "../include/Configuration.hpp"
#include "../include/Server.hpp"

//print map in pink
void Server::printMapPink() {
    for (std::map<std::string, std::vector <std::string> >::iterator it = map.begin(); it != map.end(); ++it) {
        std::cout << "\033[35m" << it->first << "\033[0m";
        std::cout << "\033[35m" << " -> " << "\033[0m";
        for(std::vector <std::string>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
            std::cout << "\033[35m" << *it2 << " |"<< " " << "\033[0m";
        std::cout << std::endl;
    }
}

void printVec(std::vector<std::string> vec) {
    for (size_t i = 0; i < vec.size(); i++) {
        std::cout << "\033[35m" << vec[i] << "|" << "\033[0m";
    }
    std::cout << std::endl;
}

void Configuration::printServerList() {
    for (size_t i = 0; i < serverVec.size(); i++) {
        serverVec[i].printMapPink();
        serverVec[i].printLocation();
        std::cout << "________________________" << std::endl;
    }
}

void printYellow(std::string str1, std::string str2) {
    std::cout << "\033[33m" << str1 << str2 << "\033[0m" << std::endl;
}
