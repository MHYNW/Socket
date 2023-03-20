//
//  client_main.cpp
//  Socket
//
//  Created by 임현우 on 2023/03/20.
//

#include "client.hpp"

int main(int argc, const char * argv[])
{
    if (argc !=4 ) {
        std::cerr << "Error, needs 3 arguments" << std::endl;
        exit(0);
    }
    int server_port = atoi(argv[2]);
    std::string file_name{argv[3]};
    std::string server_ip{argv[1]};
    SOCKET::CLIENT clinet{file_name, server_ip, server_port};
    clinet.run();
}
