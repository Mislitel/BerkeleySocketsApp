cmake_minimum_required(VERSION 3.10)
project(BerkeleySocketsApp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

add_executable(client Client.cpp)
add_executable(server TcpServer.cpp UdpServer.cpp Server.cpp Parser.cpp)