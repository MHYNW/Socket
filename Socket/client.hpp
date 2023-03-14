//
//  client.hpp
//  Socket
//
//  Created by 임현우 on 2023/03/05.
//

#ifndef client_hpp
#define client_hpp

#include "socket.hpp"

class SOCKET::CLIENT {
    const int server_port;
    const std::string server_IP;
    const std::string file_name;
    const int tcpConnect();
    size_t getFileSize();
    int fileRead(std::string& file_buf, size_t& len);
    int sendData(const int sockfd, std::string& file_buf, size_t len);
    int sendHeader(const int sockfd, size_t len);
public:
    CLIENT(const std::string& file_name, const std::string& server_IP, const int& server_port);
    int run();
};

#endif /* client_hpp */
