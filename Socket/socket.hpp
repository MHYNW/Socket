//
//  socket.hpp
//  Socket
//
//  Created by 임현우 on 2023/03/04.
//

#ifndef socket_hpp
#define socket_hpp

#pragma once
#include <cstdio>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <list>
#include <cstring>
#include <thread>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PACKET_SIZE 1024
#define HEADER_SIZE 5
#define BACKLOG 5
#define DELIMITER '%'
#define HEADER_TYPE '$'

namespace SOCKET {
    struct TCPHEADER;
    class SERVER;
    class CLIENT;
}

struct SOCKET::TCPHEADER {
    char type;
    size_t size;
};

#endif /* socket_hpp */
