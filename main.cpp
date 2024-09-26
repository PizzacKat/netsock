#include <iomanip>
#include <iostream>
#include <netsock/poll_list.hpp>
#include <netsock/tcp_listener.hpp>

#include "netsock/tcp_client.hpp"

struct tcp_message {
    std::string name;
    std::vector<std::string> args;
    std::vector<std::byte> data;
};

std::basic_ostream<std::byte> &operator<<(std::basic_ostream<std::byte> &os, const std::span<const std::byte> bytes) {
    if (std::basic_ostream<std::byte>::sentry(os))
        os.write(std::data(bytes), (std::streamsize)std::size(bytes));
    return os;
}

std::basic_istream<std::byte> &operator>>(std::basic_istream<std::byte> &is, const std::span<std::byte> bytes) {
    if (std::basic_istream<std::byte>::sentry(is))
        is.read(std::data(bytes), (std::streamsize)std::size(bytes));
    return is;
}

std::basic_ostream<std::byte> &operator<<(std::basic_ostream<std::byte> &os, const std::string_view &string) {
    os << std::as_bytes(std::span(string));
    return os;
}

std::basic_istream<std::byte> &operator>>(std::basic_istream<std::byte> &is, std::string &string) {
    std::basic_string<std::byte> str;
    is >> str;
    string.assign((const char *)str.data(), (const char *)str.data() + str.size() + 1);
    return is;
}

std::basic_ostream<std::byte> &operator<<(std::basic_ostream<std::byte> &os, const tcp_message &m) {
    os << m.name;
    for (const std::string &arg : m.args)
        os << ' ' << arg;
    os << "\r\n";
    if (m.data.empty()) {
        os << "0\r\n";
        return os;
    }
    os << std::hex << m.data.size() << std::dec << "\r\n";
    os << m.data;
    return os;
}

struct newline_t {

};

static const newline_t newline;

template <typename CharT, typename Traits>
std::basic_istream<CharT, Traits> &operator>>(std::basic_istream<CharT, Traits> &is, const newline_t ) {
    typename std::basic_istream<CharT, Traits>::sentry s(is);
    if (s) {
        CharT bytes[2];
        is.read(bytes, 2);
        if (bytes[0] != (CharT)'\r' || bytes[1] != (CharT)'\n')
            is.setstate(std::ios::failbit);
    }
    return is;
}

std::basic_istream<std::byte> &operator>>(std::basic_istream<std::byte> &is, tcp_message &m) {
    is >> m.name;
    std::string arg;
    while (is && is.peek() != '\r') {
        is >> arg;
        m.args.push_back(arg);
    }
    is >> newline;
    std::size_t size;
    is >> std::hex >> size >> std::dec;
    if (size == 0) {
        m.data.clear();
        return is;
    }
    m.data.resize(size);
    is >> m.data;
    return is;
}

enum class tcp_tunnel_type {
    closed, client, command, tunnel
};

class tcp_tunnel {
public:
    static constexpr int backlog = 1024;

    explicit tcp_tunnel(const netsock::address_family family = netsock::af_inet): _socket(family), _remote(family) {

    }

    void listen(const netsock::ip_endpoint &endpoint) {
        _socket.bind(endpoint);
        _socket.listen(backlog);
        _type = tcp_tunnel_type::command;
    }

    void process() {
        if (_type == tcp_tunnel_type::closed)
            throw std::logic_error("Not connected");
        while (_socket.socket().can_read()) {
            if (_client.socket() != netsock::socket::invalid_socket) {
                auto client = _socket.accept_client();
                client.stream() << tcp_message("DISCONNECT");
                client.close();
                continue;
            }
            _client = _socket.accept_client();
        }

        if (_type == tcp_tunnel_type::command) {
            
        }

        if (_type == tcp_tunnel_type::tunnel) {
            netsock::poll_list events{{_client.socket(), netsock::pollin}, {_remote.socket(), netsock::pollin}};
            if (events.poll() == 0)
                return;
            if (events.result(_remote.socket())) {
                auto &remote_socket = _remote.socket();
                auto &client_socket = _client.socket();
                const std::size_t bytes = remote_socket.recv(_buffer);
                client_socket.send(_buffer, bytes);
            }
            if (events.result(_client.socket())) {
                auto &remote_socket = _remote.socket();
                auto &client_socket = _client.socket();
                const std::size_t bytes = client_socket.recv(_buffer);
                remote_socket.send(_buffer, bytes);
            }
        }
    }
private:
    tcp_tunnel_type _type = tcp_tunnel_type::closed;
    netsock::tcp_client _client{netsock::socket::invalid_socket};
    netsock::tcp_listener _socket;
    netsock::tcp_client _remote;
    std::byte _buffer[1024] = {};
};

int main() {

    return 0;
}
