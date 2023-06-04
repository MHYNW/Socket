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
#include <string>
#include <cstring>
#include <thread>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PACKET_SIZE             (1024)
#define HEADER_SIZE             (5)
#define BACKLOG                 (5)
#define DELIMITER               ('%')
#define HEADER_TYPE             ('$')

const std::string FAIL_TCP_LISTEN = "Failed to Listen TCP";
const std::string FAIL_TCP_CONNECT = "Failed to Connect TCP";
const std::string FAIL_IP_LOAD = "Failed to Load IP";
const std::string FAIL_SOCKET_OPEN = "Failed to Open Socket";
const std::string FAIL_SOCKET_BIND = "Failed to Bind Socket";
const std::string FAIL_SOCKET_LISTEN = "Failed to Listen Socket";
const std::string FAIL_HEADER_RECEIVE = "Failed to Receive Header";
const std::string FAIL_DATA_RECEIVE = "Failed to Receive Data";
const std::string FAIL_DATA_SEND = "Failed to Send Data";
const std::string FAIL_DATA_LOAD = "Failed to Load Data";
const std::string FAIL_PACKET_READ = "Failed to Read Packet";
const std::string FAIL_FILE_WRITE = "Failed to Write File";
const std::string FAIL_FILE_OPEN = "Failed to Open File";
const std::string FAIL_FILE_SAVE = "Failed to Save File";
const std::string FAIL_FILE_READ = "Failed to Read File";
const std::string FAIL_SERVER_CONNECT = "Failed to Connect Server";


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
