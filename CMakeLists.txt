cmake_minimum_required(VERSION 3.29)
project(netsock)

set(CMAKE_CXX_STANDARD 20)

if (WIN32)
    link_libraries(wsock32 ws2_32)
    set(impl src/impl/windows.cpp)
elseif (LINUX)
    set(impl src/impl/linux.cpp)
endif ()

set(src src/socket.cpp
        src/ip_endpoint.cpp
        src/ip_address.cpp
        src/tcp_client.cpp
        src/tcp_listener.cpp
        src/socket_address.cpp
        src/udp_socket.cpp
        src/poll_list.cpp
        src/tcp_stream.cpp
)

if (BUILD_SHARED_LIBS)
    set(CMAKE_DEBUG_POSTFIX  "-d")
    set(CMAKE_RELEASE_POSTFIX "")
    target_compile_options(netsock PUBLIC "-fPIC")

else ()
    set(CMAKE_DEBUG_POSTFIX  "-ds")
    set(CMAKE_RELEASE_POSTFIX "-s")
endif ()

add_library(netsock
        ${src}
        ${impl}
)

target_include_directories(netsock PUBLIC include)