#include <utility>

#include "netsock/tcp/tcp_client.hpp"
#include "netsock/dns.hpp"
#include "netsock/errors.hpp"

namespace netsock {
    tcp_client::tcp_client(): m_client(invalid_socket), m_stream(m_client){

    }

    tcp_client::tcp_client(netsock::address_family family): m_client(family, socket_type::stream, tcp), m_stream(m_client){

    }

    tcp_client::tcp_client(const std::string &address, unsigned short port) : tcp_client(netsock::socket(socket_type::stream, ip_protocol::tcp)) {
        auto addresses = dns::getHostAddresses(address);
        connect(addresses, port);
    }

    tcp_client::tcp_client(socket &&client): m_client(std::move(client)), m_stream(m_client){

    }

    tcp_client::tcp_client(tcp_client &&client) noexcept: m_client(std::move(client.m_client)), m_stream(m_client) {

    }

    tcp_client &tcp_client::operator=(tcp_client &&client) noexcept {
        m_client = std::move(client.m_client);
        return *this;
    }

    void tcp_client::connect(const netsock::ip_address &address, unsigned short port) {
        m_client.connect(address, port);
    }

    void tcp_client::connect(const std::vector<ip_address> &addresses, unsigned short port) {
        m_client.connect(addresses, port);
    }

    void tcp_client::connect(const std::string &address, unsigned short port) {
        auto addresses = dns::getHostAddresses(address);
        connect(addresses, port);
    }

    [[nodiscard]] bool tcp_client::connected() const {
        return m_client.valid() && m_client.connected();
    }

    bool tcp_client::pending() {
        return m_client.poll(0, netsock::poll_mode::read);
    }

    const socket &tcp_client::client() const {
        return m_client;
    }

    socket &tcp_client::client() {
        return m_client;
    }

    retsize_t tcp_client::write(const char *data, netsock::datsize_t length, size_t offset) {
        return client().write(data, length, offset);
    }

    size_t tcp_client::write(const std::vector<char> &data) {
        return client().write(data);
    }

    retsize_t tcp_client::read(char *out, netsock::datsize_t amount, size_t offset) {
        return client().read(out, amount, offset);
    }

    std::vector<char> tcp_client::read(size_t amount) {
        return client().read(amount);
    }

    void tcp_client::close(){
        if (connected())
            m_client.shutdown();
        m_client.close();
    }

    tcp_client tcp_client::create() {
        return tcp_client(socket(socket_type::stream, ip_protocol::tcp));
    }

    tcp_client::~tcp_client() {
        close();
    }
}