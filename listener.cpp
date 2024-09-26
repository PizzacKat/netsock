#include <iostream>
#include "netsock/udp_socket.hpp"

int main() {
    netsock::udp_socket listener;
    listener.bind(netsock::ip_address::any & 9188);
    char data[256];
    netsock::ip_endpoint remote;
    listener.recvfrom(data, 256, remote);
    netsock::udp_connection connection(std::move(listener), remote);
    connection.send(data, 256);
    std::cout << data << std::endl;
}
