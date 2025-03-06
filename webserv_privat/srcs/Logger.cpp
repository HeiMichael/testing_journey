/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:11:27 by miheider          #+#    #+#             */
/*   Updated: 2025/01/22 14:27:31 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Logger.hpp"

void    Logger::log_entry() {
    time_t  rawtime;
    struct tm *timeinfo;
    char    buffer[35];
    double  processtime;

    std::clock_t start = std::clock();
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    std::clock_t end = std::clock();
    processtime = (static_cast<double>(end - start) / CLOCKS_PER_SEC) * 1000.0;
    
    strftime (buffer,35,"[%a %b %d %Y - %I:%M:%S%p %Z]", timeinfo);
    std::cout << buffer << " - IP client - " << "method" << " - uri - " << "protocol" << " - status code - " << "description - " << processtime << "ms" << std::endl;
}
