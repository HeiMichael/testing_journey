/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 23:21:41 by miheider          #+#    #+#             */
/*   Updated: 2025/03/06 10:52:54 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>
#include <vector>
#include <ctime>
#include "Configuration.hpp"
#include "Server.hpp"

#define MAX_HEADER_SIZE     32768
#define DEFAULT_HEADER_SIZE 16384

class Request {
    private:
        static unsigned int                                     _next_id;
        unsigned int                                            _id;
        std::string                                             _method;
        std::string                                             _uri;
        std::vector<std::string>                                _request_path;
        std::string                                             _query_line;
        std::vector<std::pair<std::string, std::string> >       _query;
        std::string                                             _protocol;
        std::map<std::string, std::string>                      _header;
        unsigned long                                           _content_length;
        std::string                                             _body;
        std::string                                             _host;
        std::string                                             _port;
        bool                                                    _is_cgi;
        bool                                                    _is_valid;
        bool                                                    _is_complete;
        bool                                                    _is_body;
        bool                                                    _is_query;
        bool                                                    _is_host;
        bool                                                    _is_port;
        bool                                                    _is_content_length;
        bool                                                    _is_transfer_encoding;
        unsigned short                                          _status_code;
        Server                                                  *_my_server;
        unsigned int                                            _header_size;
        unsigned int                                            _client_header_buffer_size;
        unsigned long                                           _client_max_body_size;

        void                                                    encode(std::string &to_encode);
        void                                                    validate_method();
        void                                                    validate_uri();
        void                                                    validate_protocol();
        void                                                    parse_request(std::string buffer, Configuration &ConfigFile);
        void                                                    parse_requestline(std::string& requestline);
        void                                                    validate_key(std::string& key);
        void                                                    validate_value(std::string& value);
        void                                                    parse_header(std::string header);
        size_t                                                  convert_length(std::string c_length);
        bool                                                    validate_content_length(std::string& c_length);
        void                                                    parse_body(std::string body);
    
        Request();
        // Request& operator=(const Request &other);
        Request (const Request &other);
        
    public:
        Request(std::string buffer, Configuration &ConfigFile);
        ~Request();
        
        unsigned int                                            get_id() const;
        const std::string&                                      get_method() const;
        const std::string&                                      get_uri() const;
        const std::vector<std::string>&                         get_request_path() const;
        const std::string&                                      get_query_line() const;
        const std::vector<std::pair<std::string, std::string> >&get_query() const;
        const std::string&                                      get_protocol() const;
        const std::map<std::string, std::string>&               get_header() const;
        const size_t&                                           get_content_length() const;
        const std::string&                                      get_body() const;
        const std::string&                                      get_host() const;
        const std::string&                                      get_port() const;
        bool                                                    get_is_cgi() const;
        bool                                                    get_is_valid() const;
        bool                                                    get_is_complete() const;
        bool                                                    get_is_body() const;
        bool                                                    get_is_query() const;
        bool                                                    get_is_host() const;
        bool                                                    get_is_content_length() const;
        bool                                                    get_is_transfer_encoding() const;
        unsigned short                                          get_status_code() const;
        Server                                                  *get_my_server() const;        
        unsigned int                                            get_header_size() const;
        void                                                    print() const;
};

// 400  Bad Request
// 403  Forbidden
// 422  Unprocessable Entity
// 505  HTTP version not supported
// 501  Not Implemented






