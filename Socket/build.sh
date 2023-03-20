#!/bin/bash

echo "Build Server"
g++ -std=c++17 socket.hpp server.hpp server.cpp server_main.cpp -pthread
mv ./a.out server
echo "Server Build Complete!"

echo "Build Client"
g++ -std=c++17 socket.hpp client.hpp client.cpp client_main.cpp -pthread
mv ./a.out client
echo "Client Build Complete!"
