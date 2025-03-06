/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_test.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 21:06:03 by miheider          #+#    #+#             */
/*   Updated: 2025/02/10 13:02:14 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../include/Request.hpp"

    void test_request_valid() {
        Request request1("GET /index.html HTTP/1.1\r\nHost: me\r\ntransfer-encong: me\r\nCONTENT-LENGTH: 0\r\nLeng: 1234\r\n\r\n");
        
        bool passed = (request1.get_is_valid() &&
                    request1.get_method() == "GET" &&
                    request1.get_uri() == "/index.html" &&
                    request1.get_protocol() == "HTTP/1.1" &&
                    request1.get_status_code() == 200);

        if (passed)
            std::cout << "::notice title=request_valid Request Passed::✅ request1" << std::endl;
        else
            std::cerr << "::error title=request_valid Request Failed::❌ request1" << std::endl;
    }
    
    void test_requestline_not_complete() {
        Request request2("GET /index.html \r\ntest1: me\r\ntest2: you\r\n\r\n");
        
        bool passed = (!request2.get_is_valid());

        if (passed)
            std::cout << "::notice title=requestline_not_complete Request Passed::✅ request2" << std::endl;
        else
            std::cerr << "::error title=requestline_not_complete Request Failed::❌ request2" << std::endl;
    }

    // void test_lowercase_in_method() {
    // }
    
    // void test_too_many_spaces() {
    // }
    
    // void test_special_method() {
    // }

    // void test_four_args_in_first_line() {
    // }
    
    // void test_encoded_path() {
    // }

    // void test_notallowed_char_in_path() {
    // }

    // void test_path_with_query() {
    // }
    
    // void test_query_with_duplicate_key() {
    // }
    
    // void test_empty_query() {
    // }
    
    // void test_protocol_2_0() {
    // }
    
    // void test_protocol_lowercase() {
    // }

    // void test_header_1_1_without_host() {
    // }
    
    // void test_hesder_1_0_without_host() {
    // }
    
    // void test_notallowed_char_key() {
    // }
    
    // void test_leading_space_value() {
    // }
    
    // void test_leading_space_key(){
    // }


int main() {
    std::cout << "begin tests request" << std::endl;

    test_request_valid();
    test_requestline_not_complete();
    // test_lowercase_in_method();
    // test_too_many_spaces();
    // test_special_method();
    // test_four_args_in_first_line();
    // test_encoded_path();
    // test_notallowed_char_in_path();
    // test_path_with_query();
    // test_query_with_duplicate_key();
    // test_empty_query();
    // test_protocol_2_0();
    // test_protocol_lowercase();
    // test_header_1_1_without_host();
    // test_hesder_1_0_without_host();
    // test_notallowed_char_key();
    // test_leading_space_value();
    // test_leading_space_key();
}
