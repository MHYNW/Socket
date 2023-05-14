//
//  Server.hpp
//  Socket
//
//  Created by 임현우 on 2023/03/05.
//

#ifndef Server_hpp
#define Server_hpp

#include "socket.hpp"

class SOCKET::SERVER : protected SOCKET::TCPHEADER {
    const int port;
    const std::string save_dir;
    int fileWrite(const std::string filename, const std::string& data);
    int fileSave(std::string& filedata);
    int getClientIP(const int sockfd);
    int tcpListen(sockaddr_in& server_addr);
    int recvData(const int accpt_sockfd, std::string& filedata, size_t data_len);
    int recvHeader(const int accpt_sockfd, char* header_packet, size_t head_len);
    int fileRecv(const int accpt_sockfd);
    int waitingClient(int& accept_sockfd, int& listen_sockfd,
                      sockaddr_in& server_addr, sockaddr_in& client_addr, socklen_t& len) noexcept;
public:
    SERVER(const std::string& dir, const int port);
    int run();
};

#endif /* Server_hpp */
