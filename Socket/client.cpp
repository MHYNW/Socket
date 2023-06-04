//
//  client.cpp
//  Socket
//
//  Created by 임현우 on 2023/03/05.
//

#include "client.hpp"


SOCKET::CLIENT::CLIENT(const std::string& file_name, const std::string& server_IP, const int& server_port)
        : file_name{file_name}, server_IP{server_IP}, server_port{server_port}
{
    std::clog << "File Name: " << file_name << ", " << "Server IP: " << server_IP << ", " <<
    "Server Port: " << server_port << std::endl;
}


int SOCKET::CLIENT::run()
{
    try {
        size_t filesize = getFileSize();
        std::string file_buf;
        if (fileRead(file_buf, filesize) < 0) {
            throw FAIL_FILE_READ;
        }
        std::clog << "File Name: " << file_name << ", " << "File Size: " << filesize << std::endl;
        const int sockfd = tcpConnect();
        if (sockfd < 0) {
            throw FAIL_SERVER_CONNECT;
        }
        if (sendData(sockfd, file_buf, filesize) < 0) {
            throw FAIL_DATA_SEND;
        }
    } catch (std::string exp) {
        std::cerr << exp << std::endl;
    }
    return 0;
}

size_t SOCKET::CLIENT::getFileSize()
{
    std::ifstream file;
    file.open(file_name);
    try {
        if (!file.is_open()) {
            throw FAIL_FILE_OPEN;
        }
        // move curser to end of the file //
        file.seekg(0, std::ios::end);
        // return the file size //
    } catch (std::string exp) {
        std::cerr << exp << std::endl;
    }
    return file.tellg();
}

int SOCKET::CLIENT::fileRead(std::string& file_buf, size_t& len)
{
    try {
        std::ifstream file;
        file.open(file_name);
        if (!file.is_open()) {
            throw FAIL_FILE_OPEN;
        }
        file_buf.resize(len);
        file.read(&file_buf[0], len);
        file_buf.insert(0, file_name + DELIMITER);
        // file length has changed //
        len = file_buf.size();
    } catch(std::string exp) {
        std::cerr << exp << std::endl;
    }
        return 0;
}

const int SOCKET::CLIENT::tcpConnect()
{
    sockaddr_in serveraddr;
    const int port{server_port};
    const int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    try {
        if (sockfd < 0) {
            throw FAIL_SOCKET_OPEN;
        }
        memset(reinterpret_cast<char*>(&serveraddr), 0, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        // error warning //
        serveraddr.sin_addr.s_addr = inet_addr(server_IP.c_str());
        serveraddr.sin_port = htons(port);
        if (connect(sockfd, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) < 0) {
            throw FAIL_TCP_CONNECT;
        }
        std::clog << "TCP Connect Complete" << std::endl;
    } catch (std::string exp) {
        std::cerr << exp << std::endl;
    }
    return sockfd;
}

int SOCKET::CLIENT::sendData(const int sockfd, std::string& file_buf, size_t len)
{
    char* data_buff = new char[len + 1];
    char* data_packet = new char[PACKET_SIZE + 1];
    try {
        size_t send_len{0};
        size_t data_len{len};
                // converting string to char array //
        strcpy(data_buff, file_buf.c_str());
        // send header //
        sendHeader(sockfd, len);
        // send data //
        while (true) {
        // initializing to 0 in every packet //
            memset(data_packet, 0, PACKET_SIZE + 1);
            if (data_len < 0) {
                break;
            }
            if (data_len <= PACKET_SIZE) {
                memcpy(data_packet, data_buff, data_len);
                send_len = send(sockfd, data_packet, data_len, 0);
                if (send_len < 0) {
                    std::cerr << "Error, Send #01" << std::endl;
                    throw FAIL_DATA_SEND;
                }
                else if (send_len == data_len) {
                    break;
                }
                data_buff += send_len;
                data_len -= send_len;
            }
            else {
                memcpy(data_packet, data_buff, PACKET_SIZE);
                send_len = send(sockfd, data_packet, PACKET_SIZE, 0);
                if (send_len < 0) {
                    std::cerr << "Error, Send #02" << std::endl;
                    throw FAIL_DATA_SEND;
                }
                if (send_len == data_len) {
                    break;
                }
                data_buff += send_len;
                data_len -= send_len;
            }
        }
        delete[] data_buff;
        delete[] data_packet;
    } catch (std::string exp) {
        std::cerr << exp << std::endl;
        delete[] data_buff;
        delete[] data_packet;
    }
        return 0;
}

int SOCKET::CLIENT::sendHeader(const int sockfd, size_t len)
{
    char head_type{HEADER_TYPE};
    char* head_packet = new char[HEADER_SIZE + 1];
    size_t packet_len = htonl(len);
    // defining header //
    head_packet[0] = HEADER_TYPE;
    memcpy(head_packet + 1, &packet_len, sizeof(packet_len));
    // send header //
    send(sockfd, head_packet, HEADER_SIZE, 0);
    delete[] head_packet;
    return 0;
}
