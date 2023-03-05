//
//  Server.cpp
//  Socket
//
//  Created by 임현우 on 2023/03/05.
//

#include "server.hpp"

SOCKET::SERVER::SERVER(const std::string& dir, const int port)
                        : save_dir{dir}, port{port}
{
    std::clog << "File Directory: " << dir << " ," << "Port: " << port << std::endl;
}

int SOCKET::SERVER::run()
{
    sockaddr_in server_addr, client_addr;
    socklen_t len = sizeof(client_addr);
    int listen_sockfd;
    int accept_sockfd;
    if ((listen_sockfd = tcpListen(server_addr)) < 0) {
        std::cerr << "Failed TCP Listen" << std::endl;
        return -1;
    }
    else{
        std::clog << "TCP Listen Complete" << std::endl;
    }
    while (true) {
        accept_sockfd = accept(listen_sockfd, reinterpret_cast<sockaddr*>(&client_addr), &len);
        // skip the loop until the client try to connect //
        if (accept_sockfd < 0) {
            continue;
        }
        std::thread mainthread{&SOCKET::SERVER::fileRecv, this, accept_sockfd};
        // detatch for new thread //
        mainthread.detach();
        getClientIP(accept_sockfd);
    }
    return 0;
}

int SOCKET::SERVER::tcpListen(sockaddr_in& server_addr)
{
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Failed to Open Socket" << std::endl;
        return -1;
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(this->port);
    if (bind(sockfd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
        std::cerr << "Failed to Bind" << std::endl;
        return -1;
    }
    if (listen(sockfd, BACKLOG) < 0) {
        std::cerr << "Failed to Listen" << std::endl;
        return -1;
    }
    return sockfd;
}

int SOCKET::SERVER::fileRecv(const int accpt_sockfd)
{
    std::string filedata;
    TCPHEADER header;
    char* header_packet = reinterpret_cast<char*>(&header);
    memset(&header, 0, sizeof(header));
    size_t head_len = HEADER_SIZE;
    size_t data_len;
    // read headder //
    if (recvHeader(accpt_sockfd, header_packet, head_len) < 0) {
        return -1;
    }
    // data shifting for padding bytes //
    memcpy(&header.size, &header_packet[1], sizeof(data_len));
    data_len = ntohl(header.size);
    // read Data //
    if (recvData(accpt_sockfd, filedata, data_len) < 0) {
        return -1;
    }
    fileSave(filedata);
    return 0;
}

int SOCKET::SERVER::recvHeader(const int accpt_sockfd, char* header_packet, size_t head_len)
{
    size_t recv_head;
    while (true) {
        recv_head = recv(accpt_sockfd, header_packet, head_len, 0);
        if (recv_head < 0) {
            std::cerr << " Failed to Read Header" << std::endl;
            return -1;
        }
        else if (recv_head == head_len) {
            break;
        }
        head_len -= recv_head;
        header_packet += recv_head;
    }
    return 0;
}

int SOCKET::SERVER::recvData(const int accpt_sockfd, std::string& filedata, size_t data_len)
{
    char* data_packet = new char[PACKET_SIZE + 1];
    size_t recv_data;
    while (true) {
        recv_data = recv(accpt_sockfd, data_packet, PACKET_SIZE, 0);
        if (recv_data < 0) {
            std::cerr << "Failed to Read Packet" << std::endl;
            return -1;
        }
        else if (recv_data == data_len) {
            filedata.append(data_packet, recv_data);
            break;
        }
        filedata.append(data_packet, recv_data);
        data_len -= recv_data;
    }
    delete[] data_packet;
    return 0;
}

int SOCKET::SERVER::fileSave(std::string& filedata)
{
    std::string filepath = "";
    std::string filename = "";
    auto nPos = filedata.find(DELIMITER);
    size_t filesize = filedata.size();
    if (nPos != std::string::npos) {
        filename += filedata.substr(0, nPos);
    }
    if (*(save_dir.end() - 1) == '/') {
        filepath += save_dir;
    }
    else {
        filepath += save_dir + "/";
    }
    filepath += filename;
    if (fileWrite(filepath, filedata.substr(nPos + 1)) < 0) {
        std::cerr << "Failed to File Write" << std::endl;
        return -1;
    }
    std::clog << "File: " << filepath << ", " << "File Size: " << filesize << std::endl;
    return 0;
}

int SOCKET::SERVER::fileWrite(const std::string filename, const std::string& data)
{
    std::ofstream file;
    if (data.empty()) {
        std::cerr << "Failed to Load the Data" << std::endl;
        return -1;
    }
    else {
        file.open(filename);    // error caution
        //file.open("Sended File.txt");
        if (!file.is_open()) {
            std::cerr << "Failed to Open File" << std::endl;
            return -1;
        }
        file << data;
    }
    return 0;
}


int SOCKET::SERVER::getClientIP(const int sockfd)
{
    sockaddr_in sock;
    socklen_t len = sizeof(sock);
    if (getpeername(sockfd, reinterpret_cast<sockaddr*>(&sock), &len) < 0) {
        std::cerr << "Failed to Load Client's IP Addr. " << std::endl;
        return -1;
    }
    std::clog << "Connected Client's IP Addr: " << inet_ntoa(sock.sin_addr) << std::endl;
    return 0;
}
