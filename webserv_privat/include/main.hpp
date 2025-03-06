/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 13:26:31 by jseidere          #+#    #+#             */
/*   Updated: 2025/03/04 14:28:24 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include "Configuration.hpp"
#include "Server.hpp"
#include <sys/epoll.h>
#include <sys/socket.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <string>
#include <netinet/in.h>
#include <cstring>
#include <cstdio>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include "Request.hpp"

#define MAX_EVENTS 10
#define BUFFER_SIZE 4096 //good balance between memory usage and perfomance

int non_blocking(int fd);
int startServer(Configuration ConfigFile);

