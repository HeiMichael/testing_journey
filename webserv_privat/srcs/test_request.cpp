/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_request.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:37:42 by miheider          #+#    #+#             */
/*   Updated: 2025/02/24 10:25:52 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Request.hpp"
#include <iostream>
#include <string>
#include <ctime>

int main() {
    std::cout << "++++++++++++++++++++REQUEST1++++++++++++++++++++" << std::endl;
    double process_time;
    
    std::clock_t start = std::clock();
    Request request1("GET /index.html?name=mi%20chi&location=wien HTTP/1.1\r\nHost: me\r\ntransfer-encoding: chunked\r\nLeng: 1234\r\n\r\n5\r\nHello\r\n6\r\n, this\r\n2\r\n i\r\n6\r\ns a ch\r\n6\r\nunked \r\n7\r\nmessage\r\n1\r\n!\r\n0\r\n\r\n");
    std::clock_t end = std::clock();
    process_time = static_cast<double>(end - start) * 1000.0 / CLOCKS_PER_SEC;
    std::cout << "---------main print------------" << std::endl;
    std::cout << request1.get_method() << "*" << request1.get_uri() << "*" << request1.get_protocol() << std::endl;
    const std::vector<std::string> path = request1.get_request_path();
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << "path: " << path[i] << std::endl;
    }
    const std::vector<std::pair<std::string, std::string> >& query = request1.get_query();
    for (size_t i = 0; i < query.size(); ++i) {
        std::cout << "key: " << query[i].first << "\nvalue: " << query[i].second << std::endl;
    }
    std::map<std::string, std::string> test = request1.get_header();
    for (std::map<std::string, std::string>::iterator it = test.begin(); it != test.end(); it++) {
        std::cout << "Key: " << it->first << "\tValue: " << it->second << "*" << std::endl;
    }
    std::cout << "Body: " << request1.get_body() << std::endl;
    std::cout << "status code: " << "\033[1m\033[32m" << request1.get_status_code() << "\033[0m" << std::endl;
    std::cout << "id: " << request1.get_id() << std::endl;
    std::cout << "Host: " << request1.get_is_host() << std::endl;
    std::cout << "Content-Length: " << request1.get_is_content_length() << std::endl;
    std::cout << "Transfer-encoding: " << request1.get_is_transfer_encoding() << std::endl;
    std::cout << "Content-Length: " << request1.get_content_length() << std::endl;
    std::cout << "Time to process: " << process_time << std::endl;

    std::cout << "++++++++++++++++++++REQUEST2++++++++++++++++++++" << std::endl;

    Request request2("GET /hallo/hallo HTTP/1.1\r\nHost: me\r\ntransfer-encong: me\r\nCONTENT-LENGTH: 6548\r\nLeng: 1234\r\n\r\nwas geht ab!ihr leute.");
    std::cout << "---------main print------------" << std::endl;
    std::cout << request2.get_method() << "*" << request2.get_uri() << "*" << request2.get_protocol() << std::endl;
    const std::vector<std::string> path2 = request2.get_request_path();
    for (size_t i = 0; i < path2.size(); ++i) {
        std::cout << "path2: " << path2[i] << std::endl;
    }
    const std::vector<std::pair<std::string, std::string> >& query2 = request2.get_query();
    for (size_t i = 0; i < query2.size(); ++i) {
        std::cout << "key: " << query2[i].first << "\nvalue: " << query2[i].second << std::endl;
    }
    std::map<std::string, std::string> test2 = request2.get_header();
    for (std::map<std::string, std::string>::iterator it = test2.begin(); it != test2.end(); it++) {
        std::cout << "Key: " << it->first << "\tValue: " << it->second << "*" << std::endl;
    }
    std::cout << "Body: " << request2.get_body() << std::endl;
    std::cout << "status code: " << "\033[1m\033[32m" << request2.get_status_code() << "\033[0m" << std::endl;
    std::cout << "id: " << request2.get_id() << std::endl;
    std::cout << "Host: " << request2.get_is_host() << std::endl;
    std::cout << "Content-Length: " << request2.get_is_content_length() << std::endl;
    std::cout << "Transfer-encoding: " << request2.get_is_transfer_encoding() << std::endl;
    std::cout << "Content-Length: " << request2.get_content_length() << std::endl;
}
