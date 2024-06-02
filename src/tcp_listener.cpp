#include "netsock/tcp/tcp_listener.hpp"
#include "netsock/ip_endpoint.hpp"

namespace netsock {
    tcp_listener::tcp_listener(const ip_address &address, unsigned short port): tcp_listener(ip_endpoint(address, port)){

    }

    void tcp_listener::start() {
        m_server.listen(SOMAXCONN);
        m_active = true;
    }

    void tcp_listener::start(int connections) {
        m_server.listen(connections);
        m_active = false;
    }

    void tcp_listener::stop() {
        m_server.close();
        m_active = false;
    }

    bool tcp_listener::pending() {
        return m_server.poll(0, poll_mode::read);
    }

    socket tcp_listener::accept_socket() {
        return m_server.accept();
    }

    tcp_client tcp_listener::accept_client() {
        return tcp_client(accept_socket());
    }

    const socket &tcp_listener::server() const {
        return m_server;
    }

    socket &tcp_listener::server() {
        return m_server;
    }

    bool tcp_listener::active() const {
        return m_active;
    }

    tcp_listener::tcp_listener(const ip_endpoint &endpoint) {
        m_server = {socket_type::stream, ip_protocol::tcp};
        m_server.bind(endpoint);
    }
}