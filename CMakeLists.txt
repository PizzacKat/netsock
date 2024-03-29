cmake_minimum_required(VERSION 3.27)
project(netsock VERSION 1.0.0 DESCRIPTION "Networking library")

set(CMAKE_DEBUG_POSTFIX "-d")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME}
        include/netsock/socket/socket.hpp
        src/socket.cpp
        include/netsock/types.hpp
        include/netsock/ip_address.hpp
        src/ip_address.cpp
        include/netsock/netsock.hpp
        include/netsock/tcp/tcp_client.hpp
        include/netsock/dns.hpp
        include/netsock/endian.hpp
        include/netsock/errors.hpp
        include/netsock/tcp/tcp_listener.hpp
        include/netsock/socket_address.hpp
        include/netsock/ip_endpoint.hpp
        src/endian.cpp
        src/tcp_client.cpp
        src/socket_address.cpp
        src/ip_endpoint.cpp
        src/tcp_listener.cpp
        include/netsock/endpoint.hpp
        src/endpoint.cpp
        include/netsock/select_list.hpp
        src/select_list.cpp
        include/netsock/udp/udp_client.hpp
        src/udp_client.cpp
        include/netsock/udp/udp_server.hpp
        src/udp_server.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE include)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})