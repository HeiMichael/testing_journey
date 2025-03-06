/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 22:29:50 by miheider          #+#    #+#             */
/*   Updated: 2025/03/06 10:54:54 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Request.hpp"
#include "../include/Configuration.hpp"
#include <bits/stdc++.h>

unsigned int Request::_next_id = 0;

Request::Request() {}

Request::Request(std::string buffer, Configuration &ConfigFile): 
        _id(_next_id++), _content_length(0), _is_cgi(false), _is_valid(true), 
        _is_complete(true), _is_body(false), _is_query(false), _is_host(false), _is_port(false), 
        _is_content_length(false), _is_transfer_encoding(false), _status_code(200), 
        _client_header_buffer_size(DEFAULT_HEADER_SIZE), _client_max_body_size(0) {
    parse_request(buffer, ConfigFile);
}

Request::~Request() {

}

// Request& Request::operator=(const Request &other) {
    
// }

// Request::Request (const Request &other) {
    
// }

unsigned int Request::get_id() const {
    return _id;
}

const std::string& Request::get_method() const {
    return _method;
}

const std::string& Request::get_uri() const {
    return _uri;
}

const std::vector<std::string>& Request::get_request_path() const {
    return _request_path;
}

const std::string& Request::get_query_line() const {
    return _query_line;
}

const std::vector<std::pair<std::string, std::string> >& Request::get_query() const {
    return _query;
}


const std::string& Request::get_protocol() const {
    return _protocol;
}

const std::map<std::string, std::string>& Request::get_header() const {
    return _header;
}

const size_t& Request::get_content_length() const {
    return _content_length;
}

const std::string& Request::get_body() const {
    return _body;
}

const std::string& Request::get_host() const {
    return _host;
}

const std::string& Request::get_port() const {
    return _port;
}

bool Request::get_is_cgi() const {
    return _is_cgi;
}

bool Request::get_is_valid() const {
    return _is_valid;
}

bool Request::get_is_complete() const {
    return _is_complete;
}

bool Request::get_is_body() const {
    return _is_body;
}

bool Request::get_is_query() const {
    return _is_query;
}

bool Request::get_is_host() const {
    return _is_host;
}

bool Request::get_is_content_length() const {
    return _is_content_length;
}

bool Request::get_is_transfer_encoding() const {
    return _is_transfer_encoding;
}

unsigned short Request::get_status_code() const {
    return _status_code;
}

unsigned int Request::get_header_size() const {
    return _header_size;
}

Server* Request::get_my_server() const {
    return _my_server;
}


void Request::print() const {
    std::cout << "id: " << get_id() << std::endl;
    std::cout << "method: " << get_method() << std::endl;
    std::cout << "uri: " << get_uri() << std::endl;
    // std::cout << "id: " << get_id() << std::endl;
    std::cout << "query_line: " << get_query_line() << std::endl;
    // std::cout << "id: " << get_id() << std::endl;
    std::cout << "protocol: " << get_protocol() << std::endl;
    // std::cout << "id: " << get_id() << std::endl;
    std::cout << "cont_len: " << get_content_length() << std::endl;
    std::cout << "body: " << get_body() << std::endl;
    std::cout << "cgi: " << get_is_cgi() << std::endl;
    std::cout << "\033[1m\033[32mvalid: " << get_is_valid() << "\033[0m" << std::endl;
    std::cout << "complete: " << get_is_complete() << std::endl;
    std::cout << "is_body: " << get_is_body() << std::endl;
    std::cout << "is_query: " << get_is_query() << std::endl;
    std::cout << "is_host: " << get_is_host() << std::endl;
    std::cout << "is_cont_len: " << get_is_content_length() << std::endl;
    std::cout << "is_trans_encod: " << get_is_transfer_encoding() << std::endl;
    std::cout << "\033[1m\033[32mSTATUSCODE: " << get_status_code() << "\033[0m" << std::endl;
}

void    Request::encode(std::string &to_encode) {
    const std::string hex = "0123456789ABCDEF";

    for (std::string::iterator it = to_encode.begin(); it != to_encode.end(); ++it) {
        if (*it == '%' && (it + 1) != to_encode.end() && (it + 2) != to_encode.end()) {
            char first_char = std::toupper(*(it + 1));
            char second_char = std::toupper(*(it + 2));

            size_t pos1 = hex.find(first_char);
            size_t pos2 = hex.find(second_char);

            if (pos1 != std::string::npos && pos2 != std::string::npos) {
                int result = pos1 * 16 + pos2;
                if (result >= 32 && result <= 127) {
                    char ascii_char = static_cast<char>(result);
                    std::cout << "ASCII: " << result << " ('" << ascii_char << "')" << std::endl;
                    size_t index = it - to_encode.begin();
                    to_encode.replace(index, 3, 1, ascii_char);
                    it = to_encode.begin() + index - 1;
                } else {
                    _is_valid = false;
                    _status_code = 22422;
                    std::cerr << "Non-ASCII character detected: " << result << std::endl;
                    break;
                }
            } else {
                _is_valid = false;
                _status_code = 23400;
                std::cerr << "Invalid hexadecimal characters in URI!" << std::endl;
                break;
            }
        }
    }
}

/*  - checks if method only contains uppercase letter or '-' 
    - checks if the method is allowed (by subject)*/
void Request::validate_method() {
    for (std::string::iterator it = _method.begin(); it != _method.end(); ++it) {
        if (!std::isupper(*it) && *it != '-') {
            _is_valid = false;
            _status_code = 14400;
            break;
        }
    }
    if (_method == "HEAD" || _method == "OPTIONS" || _method == "PUT" || _method == "PATCH"
            || _method == "TRACE" || _method == "CONNECT") {
        std::cout << "METHOD: " << _method << std::endl;
        _is_valid = false;
        _status_code = 44405;
        return;
    }
}

/*  +++ checks uri (for length, allowed characters, delimiters)
    +++ splits query and creates key value pairs
    +++ normalizes path and saves path in a vector
    +++ encodes uri and query
    +++ checks for cgi call */
void Request::validate_uri() {
    
    //checks if uri is too long
    if (_uri.length() > 2048) {
        _is_valid = false;
        _status_code = 15505;
    }

    //checks for not allowed characters ASCII 0-32 and 127
    for (std::string::iterator it = _uri.begin(); it != _uri.end(); ++it) {
        if ((*it >= 0 && *it <= 32) || *it == 127) {
            _is_valid = false;
            _status_code = 16400;
        }
    }
    
    //checks if the query delimiter is present (?) and splits the string
    size_t pos = _uri.find('?');
    if (!(pos == std::string::npos)) {
        _is_query = true;
        _query_line = _uri.substr(pos + 1);
        _uri.erase(_uri.begin() + pos, _uri.end());
    }

    //in case a query was detected, it checks if it is empty
    if (_is_query == true && _query_line.empty()) {
        _is_valid = false;
        _status_code = 17400;
        return;
    }

    //checks if either path or query contain not allowed characters
    const std::string allowed_path = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789/-_.~%";
    const std::string allowed_query = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!$'()*+,;:@&=?/%";

    for (std::string::iterator it = _uri.begin(); it != _uri.end(); ++it) {
        if (allowed_path.find(*it) == std::string::npos) {
            _is_valid = false;
            _status_code = 20400;
        }
    }

    for (std::string::iterator it = _query_line.begin(); it != _query_line.end(); ++it) {
        if (allowed_query.find(*it) == std::string::npos) {
            _is_valid = false;
            _status_code = 21400;
        }
    }

    //%encoding for path and query
    encode(_uri);
    encode(_query_line);

    std::cout << "uri: " << _uri << std::endl;
    std::cout << "query: " << _query_line << std::endl;

    //after encoding % - check if path is valid and check ../ -->403 Forbidden !!!
    //path: check for a valid path:
    //      +++ split the path by '/' and store the elements in a vector
    //      +++ b) normalize path: by resolving ../ and ignoring ./
    //      +++ c) Prevent Directory Traversal Attacks like eg ../../etc/passwd (it shouldnt go outside the expected directories)
    //      +++ d) check for file ending.

    //split path into segments (delimiter /) and stores them in a vector
    int path_start = 0;
    if (_uri[0] == '/')
        path_start = 1;
    while (true) {
        size_t path_end = _uri.find('/', path_start);
        if (path_end == std::string::npos) {
            _request_path.push_back(_uri.substr(path_start));
            break;
        }
        _request_path.push_back(_uri.substr(path_start, path_end - path_start));
        path_start = path_end + 1;
    }
    
    //check for ./ and ../ in path and normalize path
    for (std::vector<std::string>::iterator it = _request_path.begin(); it != _request_path.end(); ) {
        if (*it == ".") {
            it = _request_path.erase(it);
        }
        else if (*it == "..") {
            if (it == _request_path.begin()) {
                _is_valid = false;
                _status_code = 37403;
                return;
            } else {
                it = _request_path.erase(it - 1);
                it = _request_path.erase(it);
            }
        } else {
            it++;
        }
    }

    //checks if a cgi call is requested.
    //in letztes element springen und nach punkt bzw endung en .py oder .sh suchen
    std::string cgi_check =_request_path.back();
    size_t dot = cgi_check.rfind('.');

    if (dot == std::string::npos || dot == 0) {
        return;
    }

    if (cgi_check.substr(dot + 1) == "py" || cgi_check.substr(dot + 1) == "sh") {
        _is_cgi = true;
    } else if (cgi_check.substr(dot + 1) == "html") {

    } else {
        _is_valid = false;
        _status_code = 26501;
        return;
    }
    std::cout << "is_cgi: " << _is_cgi << std::endl;
    
    //query: +++ split query line by & than split by = for key-value pairs 
    //      +++a) check for duplicate keys eg name=Michi&name=Jacob some server -->200, other servers -->400 Bad Request
    //      +++b) missing key-value -->400 Bad Request
    
    //store the key-value-pairs in a vector
    if (_is_query) {
        int _query_start = 0;
        while (true) {
            size_t query_end = _query_line.find('&', _query_start);
            if (query_end == std::string::npos) {
                query_end = _query_line.length();
            }
            std::string keyvalue = _query_line.substr(_query_start, query_end - _query_start);
            std::cout << "keyvalue: " << keyvalue << std::endl;

            int start_line = 0;
            size_t divide = keyvalue.find('=', start_line);
            if (divide == std::string::npos) {
                divide = keyvalue.length();
                _is_valid = false;
                _status_code = 23400;
                return;
            }
            std::string key = keyvalue.substr(start_line, divide - start_line);
            std::string value = keyvalue.substr(divide - start_line + 1, keyvalue.length());
            std::cout << "key: " << key << "\nvalue: " << value << std::endl;
            _query.push_back(std::make_pair(key, value));

            if (query_end == _query_line.length()) {
                break;
            }
            _query_start = query_end + 1;
        }
    }

    //checks if a key or value is empty;
    for (size_t i = 0; i < _query.size(); i++) {
        if (_query[i].first.empty() || _query[i].second.empty()) {
            _is_valid = false;
            _status_code = 22400;
            return;
        }
    }

    //checks for duplicate keys in query (some server 200, some server 400 (we decided for 400))
    for (size_t i = 0; i < _query.size(); i++) {
        for (size_t j = i + 1; j < _query.size(); j++) {
            if (_query[i].first == _query[j].first) {
                _is_valid = false;
                _status_code = 25400;
                return;
            }
        }
    }  
}

/*  - checks for the correct protocol*/
void Request::validate_protocol() {
    if (_protocol.compare(0, 5, "HTTP/") != 0 || _protocol[6] != '.') {
        _is_valid = false;
        _status_code = 18400;
        return;
    }
    if (_protocol[5] != '1' && (_protocol[7] != '1' || _protocol[7] != '0')) {
        _is_valid = false;
        _status_code = 19505;
        return;
    }
}

/* This method parses the request by splitting it up in a requestline (first \r\n),
headers (\r\n\r\n) and body. each section is sent to its specific parser method
to check its syntax. */
void Request::parse_request(std::string buffer, Configuration &ConfigFile) {
    size_t      line_end;
    std::string request_line;
    std::string headers;
    std::string body;

    std::cout << "id:" << get_id() << std::endl;
    std::cout << "----------requestline----------" << std::endl;
    //extract requestline
    line_end = buffer.find("\r\n");
    std::cout << "line_end: " << line_end << std::endl;
    if (line_end == std::string::npos) {    //if no \r\n;
        _is_valid = false;
        _status_code = 10400;
        return;
    }
    request_line = buffer.substr(0, line_end);
    std::cout << "Requestline: " << request_line << std::endl;
    parse_requestline(request_line);
    
    //extract headers
    std::cout << "------------headers------------" << std::endl;
    //check for client_header_buffer_size 2k; -->400 Bad Request
    size_t  header_start = line_end + 2;    //skip \r\n from request_line
    
    line_end = buffer.find("\r\n\r\n");
    std::cout << "header_start: " << header_start << "\nline_end: " << line_end << std::endl;
    if (line_end == std::string::npos) {    //if no \r\n\r\n;
        _is_valid = false;
        _status_code = 11400;
        return;
    }
    std::cout << "Length: " << line_end - header_start << std::endl;
    if (line_end - header_start > MAX_HEADER_SIZE) {
        _is_valid = false;
        _status_code = 47400;
        return;
    }
    headers = buffer.substr(header_start, line_end - header_start);
    _header_size = line_end - header_start;
    parse_header(headers);
    _my_server = ConfigFile.getServer(_host, _port);
    std::vector<std::string> chbs = _my_server->getKey("client_header_buffer_size");
    if (!chbs.empty() && !chbs[0].empty()) {
        _client_header_buffer_size = std::atoi(chbs[0].c_str());
    }
    if (_header_size > _client_header_buffer_size) {
        _is_valid = false;
        _status_code = 49400;
        return;
    }

    //extract body
    std::cout << "--------------body-------------" << std::endl;
    //get client_body_buffer_size from server class
    std::vector<std::string> cmbs = _my_server->getKey("client_max_body_size");
    if (!cmbs.empty() && !cmbs[0].empty()) {
        _client_max_body_size = std::atoll(cmbs[0].c_str());
    }
    std::cout << "client_max_body_size: " << _client_max_body_size << std::endl;
    
    // check for client_max_body_size 10M; -->413 Payload Too Large 
    size_t  body_start = line_end + 4;      //skip \r\n\r\n
    if (buffer.size() - body_start > _client_max_body_size) {
        _is_valid = false;
        _status_code = 50413;
    }
    
    if (body_start == buffer.size()) {
        _is_body = false;
    } else {
        _is_body = true;
        _body = buffer.substr(body_start);
        std::cout << "Body: " << _body << std::endl;
        parse_body(_body);
    }
    std::cout << "--------------end--------------" << std::endl;
}

void Request::parse_requestline(std::string& requestline) {
    std::istringstream  rl(requestline);
    std::string         extra;
    size_t              counter = 0;

    //checks spaces in requestline
    counter = std::count(requestline.begin(), requestline.end(), ' ');
    std::cout << "space counter = " << counter << std::endl;
    if (counter != 2) {
        _is_valid = false;
        _status_code = 12400;
        return;
    }

    //stores method, uri, protocol in its attributes and checks if exactly three strings exist
    rl >> _method >> _uri >> _protocol >> extra;
    if (_method.empty() || _uri.empty() || _protocol.empty() || !(extra.empty())) {
        _is_valid = false;
        _status_code = 13400;
        return;
    }

    validate_method();
    validate_uri();
    validate_protocol();
}

    //check if the method is supported/allowed by configuration file.

    
    //+++check for extra spaces (exactly two are mandatory in this line)
    //+++if one statement is missing or more than 3 -->400 Bad Request

    //method:   - not supported (check in server class) -->501 Not Implemented
    //          +++ only uppercase allowed (get = wrong) -->400 Bad Request
    //          +++ missing method -->400 Bad Request
    //          +++ only uppercase (A-Z) and "-" allowed -->400 Bad Request

    //uri:      +++ missing uri -->400 Bad Request
    //          +++ not allowed characters -->400 Bad Request
    //              +++ not allowed: ASCII 0-32 and 127
    //              +++ no special characters (<>"#%{}|\^~[]`) except if encoded (eg %30,..)
    //              +++ no \t \n \r \0
    //              +++ spaces are not allowed (except for %20)
    //              +++ ?(query) and #(fragment) is not allowed in the path
    //          +++ too long (max 2048 characters) -->505 URI Too Long
    //      +++ extract query (?)
    //          +++ only first ? is delimiter (rest is part of the string)
    //          +++ empty query string -->400 Bad Request
    //          +++ missing key-value -->400 Bad Request
    //          +++ duplicate keys in strings: eg name=Michi&name=Jacob some server -->200, other servers -->400 Bad Request
    //          +++ percent encoded special characters (query)
    //          +++ percent encoded special characters (path)
    //          +++ query section: allowed characters: a-z A-Z 0-9 - . _ ~ --- not allowed characters -->400 Bad Request/syntax ok, but semantically not ok -->422 Unprocessable Entity
    //          +++ reserved characters: ! $ & ' ( ) * + , ; = : @ / ?
    //          +++ # fragments are not sent to the server

    //protocol  +++ missing protocol -->400 Bad Request
    //          +++ typo (eg HTP/1.1) -->400 Bad Request
    //          +++ not supported version -->505 HTTP version not supported
    //          +++ exact format (HTTP/x.x   x=0-9)
    //          +++ only uppercase, no spaces, no special characters

    //general   +++ spaces at the beginning, at the end or multiple space inbetween are not allowed -->400 Bad Request
    //          +++ in general: ASCII 0-32 and 127 are not allowed -->400 Bad Request
    //          +++ missinf \r\n\r\n at the end -->400 Bad Request
    

void Request::validate_key(std::string& key) {
    if (key.empty()) {                          //empty key is not allowed
        _is_valid = false;
        _status_code = 29400;
        return;
    }
    for (std::string::iterator it = key.begin(); it != key.end(); ++it) {
        if (!std::isalnum(static_cast<unsigned char>(*it)) && *it != '-') {
            _is_valid = false;
            _status_code = 27400;
            return;
        } else {
            *it = std::tolower(*it);
        }
    }
}

void Request::validate_value(std::string& value) {
    if (value[0] == ' ') {
        _is_valid = false;
        _status_code = 30400;
        return;
    }
    size_t trim_end = value.find_last_not_of(" \t");
    value = value.substr(0, trim_end + 1);
    for (std::string::iterator it = value.begin(); it != value.end(); ++it) {
        if (!std::isprint(static_cast<unsigned char>(*it)) && *it != '\t' && *it != ' ') {
            _is_valid = false;
            _status_code = 28400;
            return ;
        }
    }
}

void Request::parse_header(std::string header) {
    // key-value pairs -->400 Bad Request

    // +++missing colon -->400 Bad Request
    // +++space before colon (eg Content-Length : 123) -->400 Bad Request
    // +++leading spaces in values -->400 Bad Request
    // +++each header on single line, otherwise -->400 Bad Request
    // +++header names: only alphanumeric and '-', otherwise -->400 Bad Request
    // +++Host header = required (in HTTP/1.1), otherwise -->400 Bad Request
    // if POST or PUT: either Content-Length or Transfer-Encoding must be present, otherwise --> 411 Length Required
    // Chunked encoding in HTTP/1.0 not supported

    // +++allowed characters key: A-Z a-z 0-9 - (case insensitive)
    // +++not allowed key: Whitespace, /t, : or @!#$%^&*()+=[]{};'"<>?/\|~`
    // +++header value: allowed: any printable US-ASCII char (0x20-0x7E), except controll char
    // +++header value: special handling: - leading spaces are not allowed -->400 Bad Request
    // +++header value: not allowed: leading spaces, control characters (except HTAB \t and SP space) and \r or \n only at the end
    // +++header value: can be empty
    // +++header section must end with \r\n\r\n, otherwise -->400 Bad Request
    
    // +++ each header: name, colon(:), whitespace and value
    //     +++ header names are case-insensitive (Host = host)
    //     +++ no line breaks (single header, single line brake -->end of line)
    
    // +++ header name:
    //     +++ must not start or end with whitespace

    // +++ header value:
    //     +++ any text, but ASCII 0-32 and 127 forbidden unless encoded
    
    // +++ header termination:
    //     +++ each header ends with \r\n
    //     +++ header section terminates with \r\n\r\n

    // (if confic file handles header length -->400 Bad Request)
    std::cout << "Headers: \n" << header << std::endl;
    size_t start_line = 0;
    size_t end_line = header.find("\r\n");
    while (end_line != std::string::npos) {
        std::string pair = header.substr(start_line, end_line - start_line);
        size_t colon = pair.find(':');
        if (header[start_line + colon + 1] != ' ') {
            _is_valid = false;
            _status_code = 33400;
            return;
        }
        if (colon != std::string::npos) {
            std::string key = pair.substr(0, colon);
            validate_key(key);
            std::string value = pair.substr(colon + 2);
            validate_value(value);
            _header[key] = value;
        } else {                    //no : found
            _is_valid = false;
            _status_code = 32400;
            return ;
        }

        start_line = end_line + 2;
        end_line = header.find("\r\n", start_line);
    }

    // Handle the last key-value pair
    if (start_line < header.length()) {
        std::string pair = header.substr(start_line);
        size_t colon = pair.find(':');
        if (header[start_line + colon + 1] != ' ') {
            _is_valid = false;
            _status_code = 34400;
            return;
        }
        if (colon != std::string::npos) {
            std::string key = pair.substr(0, colon);
            validate_key(key);
            std::string value = pair.substr(colon + 2);
            validate_value(value);
            _header[key] = value;
        } else {                            //no : found
            _is_valid = false;
            _status_code = 35400;
            return ;
        }
    }
    for (std::map<std::string, std::string>::iterator it = _header.begin(); it != _header.end(); ++it) {
        if (it->first == "host") {
            _is_host = true;
            _is_port = true;
            size_t colon = it->second.find(":");
            _host = it->second.substr(0, colon);
            _port = it->second.substr(colon + 1/* , it->second.length() */);
            std::cout << "Host: " << _host << "\nPort: " << _port << std::endl;
        }
        if (it->first == "content-length") {
            _is_content_length = true;
            _content_length = std::atol((it->second).c_str());
        }
        if (it->first == "transfer-encoding") {
            if (it->second == "chunked") {
                _is_transfer_encoding = true;
            }
        }
    }
    if (!_is_host && !_is_port) {
        _host = "no_host";
        _port = "no_port";
    }
    if (_protocol == "HTTP/1.1") {          //host header in HTTP/1.1 mandatory
        if (!_is_host) {
            _is_valid = false;
            _status_code = 36400;
        }
        
        for (std::map<std::string, std::string>::iterator it = _header.begin(); it != _header.end(); ++it) {
            std::cout << "header-key: " << it->first << "\theader-value: " << it->second << std::endl;
        } 
    }
    // if
}

size_t convert_to_uint(std::string hex_string) {  // convert hex to size_t 
    size_t converted;
    std::istringstream hss(hex_string);
    hss >> std::hex >> converted;
    return converted;
}

size_t Request::convert_length(std::string c_length) {
    std::istringstream css(c_length);
    long converted;
    css >> converted;
    return static_cast<unsigned int>(converted);
}

bool Request::validate_content_length(std::string& c_length) {
    if (c_length.empty() || c_length.length() > 10) {
        _is_valid = false;
        _status_code = 38400;
        return false;
    }
    for (std::string::iterator it = c_length.begin(); it != c_length.end(); ++it) {
        if (!isdigit(*it)) {
            _is_valid = false;
            _status_code = 39400;
            return false;
        }
    }
    
    if (c_length.length() == 10 && c_length > "4294967295") {
        _is_valid = false;
        _status_code = 40400;
        return false;
    }
    
    
    //check if the bodylength exceeds the value in the config file -->413 Payload too high
    return true;
}

void Request::parse_body(std::string body) {
    // ends when connection is closed (HTTP1.0)
    // if Content-header ("Content-Length:") is included, it specifies how much the webserver should read (HTTP/1.0 and HTTP/1.1)
    // if Transfer-Encoding: chunked is present, body is sent in chunks, first length (hexadecimal) followed by data, end with 0 (HTTP/1.1)
    // if both are not present, end of connection determines the end of body (HTTP/1.1)
    // POST PUT and PATCH requests typically have a body (can be empty)
    std::string c_length;

    if (get_is_body() && get_is_content_length() && !get_is_transfer_encoding()) { //only Content-header is included
    //check if config limits body!!!
        for (std::map<std::string, std::string>::iterator it = _header.begin(); it != _header.end(); ++it) {
            if (it->first == "content-length") {
                c_length = it->second;
                std::cout << "Länge des body: " << c_length << std::endl;
                if (validate_content_length(c_length)) {
                    _content_length = convert_length(c_length);
                } else {
                    return ;
                }
            }
        }
    }

    else if (get_is_body() && !get_is_content_length() && get_is_transfer_encoding()) { //only transfer-encoding included
        std::cout << "protocol_oida: " << get_protocol() << std::endl;
        if (get_protocol() == "HTTP/1.0") {
            _is_valid = false;
            _status_code = 42400;
            return;
        } else {
            size_t number = 4294967295;
            std::string content;
            size_t start_line = 0;
            while (number != 0) {
                size_t end_line = body.find("\r\n", start_line);
                if (end_line == std::string::npos) {
                    _is_valid = false;
                    _status_code = 43400; // Bad Request (Incomplete Chunk)
                    return;
                }
                std::string hexa = body.substr(start_line, end_line - start_line);
                number = convert_to_uint(hexa);
                
                if (number == 0)
                    break;
                
                start_line = end_line + 2;
                end_line = body.find("\r\n", start_line + number);
                if (end_line == std::string::npos) {
                    _is_valid = false;
                    _status_code = 44400; // Bad Request (Incomplete Chunk)
                    return;
                }
                content += body.substr(start_line, number);
                start_line = end_line + 2;                
            }
            _body = content;
            std::cout << "Decoded Chunked Body: " << content << std::endl;
        }
    }

    else if (get_is_body() && get_is_content_length() && get_is_transfer_encoding()) { //+++both are included
        _is_valid = false;
        _status_code = 41400;
        return;
    }
    else if (get_is_body() && !get_is_content_length() && !get_is_transfer_encoding()) {  //nothing is included
        _is_valid = false;
        _status_code = 45400;
        return;
    }
}

//50400
