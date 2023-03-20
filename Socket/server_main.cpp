//
//  server_main.cpp
//  Socket
//
//  Created by 임현우 on 2023/03/20.
//

#include "server.hpp"

int main(int argc, const char * argv[]) {
    if (argc != 3) {
        std::cerr << "Error, needs 2 arguments" << std::endl;
        exit(0);
    }
    int port = atoi(argv[1]);
    std::string file_name{argv[2]};
    SOCKET::SERVER server{file_name, port};
    server.run();
}
