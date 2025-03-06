/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 10:47:07 by miheider          #+#    #+#             */
/*   Updated: 2025/01/22 10:32:19 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <ctime>
#include <iostream>

/* format of the logger entries:

1) Access Log (includes every request, regardless of wheather it was successful or not):
    a) date and time:   [YYYY-MM-DD HH:MM:SS]   --- eg 2025-01-14 14:13:22
    b) client-import:                           --- eg 127.0.0.1
    c) method:                                  --- eg GET, POST, DELETE, ...
    d) URI:                                     --- eg /index.html
    e) Protocol:                                --- eg HTTP/1.1,...
    f) status code of response:                 --- eg 200, 404, ...
    g) description                              --- OK,...
    h) time to process                          --- in ms

2) Error Log (only errors with detailed description):
    a) date and time:   [YYYY-MM-DD HH:MM:SS timezone]   --- eg 2025-01-14 14:30:31
    b) ERROR:           "ERROR:"
    c) description:     invalid input data, server-side script execution issues, internal server errors,...
 */

class Logger {
    public:
        void    log_entry();

    private:
        //Logger();
        //Logger (Request request);
        //Logger (Response response);
        //~Logger();
        //Logger& operator=(const Logger& other);
        //Logger (const Logger& other);
        
};
