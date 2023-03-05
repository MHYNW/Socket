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
    int getFileSize();
    int fileRead(std::string& file_buf, int& len);
    int sendData(const int sockfd, std::string& file_buf, int len);
public:
    CLIENT(const std::string& file_name, const std::string& server_IP, const int& server_port);
    int run();
};

#endif /* client_hpp */
