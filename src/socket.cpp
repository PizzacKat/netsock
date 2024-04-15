#include "netsock/socket/socket.hpp"
#include "netsock/errors.hpp"
#include "netsock/endpoint.hpp"
#include <climits>
#include <csignal>
#include <algorithm>

#if defined(__WIN32__) || defined(__WIN64__)
class autoinit {
public:
    autoinit(){
        WSADATA data{};
        int res = WSAStartup(MAKEWORD(2, 2), &data);
        if (res != 0)
            throw netsock::socket_exception(LAST_ERROR(), "WSAStartup");
    }

    ~autoinit(){
        WSACleanup();
    }
} autoinit;
#endif

namespace netsock {
    socket_address socket::serialize(const netsock::endpoint &endpoint) const {
        if (endpoint.type() != typeid(ip_endpoint))
            throw std::invalid_argument("Endpoint must be an ip endpoint");
        netsock::ip_endpoint ip = endpoint.cast<netsock::ip_endpoint>();
        auto address = ip.address;
        if (address.family() == inet && family() == inet6) {
            address = address.map_to_ipv6();
            return ip_endpoint(address, ip.port).serialize();
        }
        return endpoint.serialize();
    }

    socket::socket() {
        m_socket = netsock::invalid_socket;
    }

    socket::socket(enum address_family af, socket_type type, ip_protocol protocol): m_family(af), m_type(type), m_protocol(protocol), m_localEndpoint(), m_remoteEndpoint() {
        m_socket = ::socket(af, type, protocol);
    }

    socket::socket(socket_type type, ip_protocol protocol): socket(inet6, type, protocol) {
        set_option(option::ipv6::ipv6_only, 0);
    }

    socket::socket(socket_t socket, bool connected): m_localEndpoint(), m_remoteEndpoint() {
        m_socket = socket;
        m_connected = connected && valid();
        if (valid()) {
#if defined(__WIN32__) || defined(__WIN64__)
            auto info = get_option<WSAPROTOCOL_INFO>(netsock::option::socket::protocol_info);
            m_family = (netsock::address_family) info.iAddressFamily;
            m_type = (netsock::socket_type) info.iSocketType;
            m_protocol = (netsock::ip_protocol) info.iProtocol;
#elif defined(__unix__)
            m_family = (netsock::address_family) get_option<int>(netsock::option::domain);
            m_type = (netsock::socket_type) get_option<int>(netsock::option::type);
            m_protocol = (netsock::ip_protocol) get_option<int>(netsock::option::protocol);
#endif
        }
    }

    socket::socket(netsock::socket &&socket) noexcept {
        m_socket = socket.m_socket;
        m_localEndpoint = socket.m_localEndpoint;
        m_remoteEndpoint = socket.m_remoteEndpoint;
        m_connected = socket.m_connected;
        m_family = socket.m_family;
        m_type = socket.m_type;
        m_protocol = socket.m_protocol;
        m_listening = socket.m_listening;
        m_closed = socket.m_closed;
        socket.m_socket = netsock::invalid_socket;
        socket.m_connected = false;
        socket.m_listening = false;
    }

    socket &socket::operator=(netsock::socket &&socket) noexcept {
        if (connected())
            shutdown();
        close();
        m_socket = socket.m_socket;
        m_localEndpoint = socket.m_localEndpoint;
        m_remoteEndpoint = socket.m_remoteEndpoint;
        m_connected = socket.m_connected;
        m_family = socket.m_family;
        m_type = socket.m_type;
        m_protocol = socket.m_protocol;
        m_listening = socket.m_listening;
        m_closed = socket.m_closed;
        socket.m_socket = netsock::invalid_socket;
        socket.m_connected = false;
        socket.m_listening = false;
        return *this;
    }

    socket_t socket::handle() const {
        return m_socket;
    }

    void socket::connect(const ip_address &address, unsigned short port) {
        connect(ip_endpoint(address, port));
    }

    void socket::connect(const netsock::endpoint &endpoint) {
        if (m_connected)
            throw invalid_operation("Socket already connected");
        socket_address address = serialize(endpoint);
        int res = ::connect(handle(), (sockaddr *)address.data(), (int)address.size());
        if (res == -1){
            int err = LAST_ERROR();
            throw socket_exception(err, "connect");
        }
        m_connected = true;
        m_remoteEndpoint = ip_endpoint(address);
    }

    void socket::connect(const std::vector<ip_address> &addresses, unsigned short port) {
        for (const ip_address &address : addresses) {
            try {
                connect(address, port);
                return;
            } catch (const socket_exception &exception){
                if (exception.code == socket_error::CONNREFUSED)
                    continue;
                throw;
            }
        }
    }

    retsize_t socket::write(const char *data, datsize_t length, size_t offset) {
#if defined(__WIN32__) || defined(__WIN64__)
        auto res = send(handle(), data + offset, length, 0);
#else
        auto res = send(handle(), data + offset, length, MSG_NOSIGNAL);
#endif
        if (res == -1){
            int err = LAST_ERROR();
            update_after_error(err);
            return 0;
        }
        return res;
    }

    size_t socket::write(const std::vector<char> &data) {
        size_t sent = 0;
        while (sent < data.size()) {
            auto res = write(data.data(), (datsize_t)std::min(data.size() - sent, (size_t)INT_MAX), sent);
            sent += res;
            if (res != INT_MAX) break;
        }
        return sent;
    }

    retsize_t socket::write_to(const endpoint &endpoint, const char *data, datsize_t length, size_t offset) {
        socket_address address = serialize(endpoint);
#if defined(__WIN32__) || defined(__WIN64__)
        auto res = sendto(handle(), data + offset, length, 0, (sockaddr *)address.data(), (int)address.size());
#else
        auto res = sendto(handle(), data + offset, length, MSG_NOSIGNAL, (sockaddr *)address.data(), (int)address.size());
#endif
        if (res == -1){
            int err = LAST_ERROR();
            update_after_error(err);
            throw socket_exception(err, "sendto");
        }
        return res;
    }

    size_t socket::write_to(const endpoint &endpoint, const std::vector<char> &data) {
        size_t sent = 0;
        while (sent < data.size()) {
            auto res = write_to(endpoint, data.data(), (int) std::min(data.size() - sent, (size_t) INT_MAX), sent);
            sent += res;
            if (res != INT_MAX) break;
        }
        return sent;
    }

    retsize_t socket::read(char *out, datsize_t amount, size_t offset) {
        auto res = recv(handle(), out + offset, amount, 0);
        if (res == 0)
            close();
        if (res == -1){
            int err = LAST_ERROR();
            update_after_error(err);
            return 0;
        }
        return res;
    }

    std::vector<char> socket::read(size_t amount) {
        size_t received = 0;
        std::vector<char> data(amount);
        while (received < amount) {
            auto res = read(data.data(), (int)std::min(amount - received, (size_t)INT_MAX), received);
            received += res;
            if (res != INT_MAX) break;
        }
        return {data.begin(), data.begin() + (signed)received};
    }

    retsize_t socket::read_from(ip_endpoint &endpoint, char *out, datsize_t amount, size_t offset) {
        socket_address storage(family());
        auto len = (socklen_t)storage.size();
        auto res = recvfrom(handle(), out + offset, amount, 0, (sockaddr *)storage.data(), &len);
        if (res == -1){
            int err = LAST_ERROR();
            update_after_error(err);
            throw socket_exception(err, "recvfrom");
        }
        socket_address address(storage.family(), len);
        std::copy(storage.data(), storage.data() + len, address.data());
        endpoint = netsock::ip_endpoint(address);
        return res;
    }

    std::vector<char> socket::read_from(ip_endpoint &endpoint, size_t amount) {
        std::vector<char> data(amount);
        auto res = read_from(endpoint, data.data(), (int) amount);
        return {data.begin(), data.begin() + res};
    }

    void socket::bind(const netsock::endpoint &endpoint) {
        socket_address address = serialize(endpoint);
        int res = ::bind(handle(), (sockaddr *)address.data(), (int)address.size());
        if (res == -1){
            int err = LAST_ERROR();
            throw socket_exception(err, "bind");
        }
        m_localEndpoint = ip_endpoint(address);
    }

    void socket::listen(int backlog) {
        if (m_localEndpoint == nullptr)
            throw invalid_operation("Socket not bound to an endpoint");
        int res = ::listen(handle(), backlog);
        if (res == -1){
            int err = LAST_ERROR();
            throw socket_exception(err, "listen");
        }
        m_listening = true;
    }

    socket socket::accept() {
        socket_address storage(family());
        auto len = (socklen_t)storage.size();
        socket_t sock = ::accept(handle(), (sockaddr *)storage.data(), &len);
        if (sock == netsock::invalid_socket){
            int err = LAST_ERROR();
            throw socket_exception(err, "accept");
        }
        socket_address address(storage.family(), len);
        std::copy(storage.data(), storage.data() + len, address.data());
        socket remote{sock};
        remote.m_localEndpoint = m_localEndpoint;
        remote.m_remoteEndpoint = netsock::ip_endpoint(address);
        return remote;
    }

    void socket::select(std::vector<const socket *> *read, std::vector<const socket *> *write,
                        std::vector<const socket *> *error, int timeout) {
        fd_set read_set, write_set, error_set;
        FD_ZERO(&read_set);
        FD_ZERO(&write_set);
        FD_ZERO(&error_set);
        socket_t greatest = 0;
        if (read != nullptr)
            for (const socket *socket : *read) {FD_SET(socket->handle(), &read_set); greatest = greatest < socket->handle() ? socket->handle() : greatest;}
        if (write != nullptr)
            for (const socket *socket : *write) {FD_SET(socket->handle(), &write_set); greatest = greatest < socket->handle() ? socket->handle() : greatest;}
        if (error != nullptr)
            for (const socket *socket : *error) {FD_SET(socket->handle(), &error_set); greatest = greatest < socket->handle() ? socket->handle() : greatest;}
        int res;
        timeval *tval = nullptr;
        timeval time = timeval();
        if (timeout >= 0) {
            timeout *= 1000;
            time.tv_sec = (long) (timeout / 1000000);
            time.tv_usec = (long) (timeout % 1000000);
            tval = &time;
        }
        res = ::select(
                greatest,
                &read_set,
                &write_set,
                &error_set,
                tval);
        if (res == -1){
            int err = LAST_ERROR();
            throw socket_exception(err, "select");
        }
        if (read != nullptr) {
            std::vector<const socket *> readOut;
            for (const socket *socket: *read) {
                if (FD_ISSET(socket->handle(), &read_set))
                    readOut.push_back(socket);
            }
            *read = readOut;
        }
        if (write != nullptr) {
            std::vector<const socket *> writeOut;
            for (const socket *socket: *write) {
                if (FD_ISSET(socket->handle(), &write_set))
                    writeOut.push_back(socket);
            }
            *write = writeOut;
        }
        if (error != nullptr) {
            std::vector<const socket *> errorOut;
            for (const socket *socket: *error) {
                if (FD_ISSET(socket->handle(), &error_set))
                    errorOut.push_back(socket);
            }
            *error = errorOut;
        }
    }

    void socket::select(select_list *read, select_list *write, select_list *error, int timeout) {
        fd_set read_set, write_set, error_set;
        FD_ZERO(&read_set);
        FD_ZERO(&write_set);
        FD_ZERO(&error_set);
        if (read != nullptr)
            for (const socket *socket : read->sockets())
                FD_SET(socket->handle(), &read_set);
        if (write != nullptr)
            for (const socket *socket : write->sockets())
                FD_SET(socket->handle(), &write_set);
        if (error != nullptr)
            for (const socket *socket : error->sockets())
                FD_SET(socket->handle(), &error_set);
        int res;
        timeval *tval = nullptr;
        timeval time = timeval();
        if (timeout >= 0) {
            timeout *= 1000;
            time.tv_sec = (long) (timeout / 1000000);
            time.tv_usec = (long) (timeout % 1000000);
            tval = &time;
        }
        res = ::select(
                (int)std::max<socket_t>({read ? read->max() : 0, write ? write->max() : 0, error ? error->max() : 0}),
                &read_set,
                &write_set,
                &error_set,
                tval);
        if (res == -1){
            int err = LAST_ERROR();
            throw socket_exception(err, "select");
        }
        if (read != nullptr) {
            std::vector<const socket *> readOut;
            for (const socket *socket: read->sockets()) {
                if (FD_ISSET(socket->handle(), &read_set))
                    readOut.push_back(socket);
            }
            read->sockets() = readOut;
        }
        if (write != nullptr) {
            std::vector<const socket *> writeOut;
            for (const socket *socket: write->sockets()) {
                if (FD_ISSET(socket->handle(), &write_set))
                    writeOut.push_back(socket);
            }
            write->sockets() = writeOut;
        }
        if (error != nullptr) {
            std::vector<const socket *> errorOut;
            for (const socket *socket: error->sockets()) {
                if (FD_ISSET(socket->handle(), &error_set))
                    errorOut.push_back(socket);
            }
            error->sockets() = errorOut;
        }
    }

    void socket::shutdown() {
#if defined(__WIN32__) || defined(__WIN64__)
        int res = ::shutdown(handle(), SD_BOTH);
#elif defined(__unix__)
        int res = ::shutdown(handle(), SHUT_RDWR);
#endif
        if (res == -1){
            int err = LAST_ERROR();
            if (err != socket_error::NOTSOCK)
                throw socket_exception(err, "shutdown");
        }
    }

    void socket::close() {
        if (!closed()) {
#if defined(__WIN32__) || defined(__WIN64__)
            closesocket(handle());
#elif defined(__unix__)
            ::close(handle());
#endif
        }
        m_connected = false;
        m_listening = false;
        m_closed = true;
    }

    bool socket::valid() const{
        return handle() != netsock::invalid_socket;
    }

    address_family socket::family() const{
        return m_family;
    }

    socket_type socket::type() const{
        return m_type;
    }

    ip_protocol socket::protocol() const{
        return m_protocol;
    }

    bool socket::connected() const {
        return m_connected;
    }

    bool socket::listening() const {
        return m_listening;
    }

    bool socket::closed() const {
        return m_closed;
    }

    endpoint socket::local_endpoint() const {
        if (m_localEndpoint == nullptr){
            socket_address address(family());
            auto len = (socklen_t)address.size();
            int res = getsockname(handle(), (sockaddr *)address.data(), &len);
            if (res == -1)
                throw socket_exception(LAST_ERROR(), "getsockname");
            return ip_endpoint(address);
        }
        return m_localEndpoint;
    }

    endpoint socket::remote_endpoint() const {
        return m_remoteEndpoint;
    }

    bool socket::poll(ssize_t timeout, poll_mode mode) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(m_socket, &fds);
        int res;
        if (timeout != -1) {
            timeout *= 1000;
            timeval time = timeval();
            time.tv_sec = (long) (timeout / 1000000);
            time.tv_usec = (long) (timeout % 1000000);
            res = ::select(
                    (int)(m_socket + 1),
                    mode == poll_mode::read ? &fds : nullptr,
                    mode == poll_mode::write ? &fds : nullptr,
                    mode == poll_mode::error ? &fds : nullptr,
                    &time);
        } else {
            res = ::select(
                    (int)(m_socket + 1),
                    mode == poll_mode::read ? &fds : nullptr,
                    mode == poll_mode::write ? &fds : nullptr,
                    mode == poll_mode::error ? &fds : nullptr,
                    nullptr);
        }
        if (res == -1){
            int err = LAST_ERROR();
            update_after_error(err);
            throw socket_exception(err, "select");
        }
        return res != 0 && FD_ISSET(m_socket, &fds);
    }

    bool socket::operator==(const socket &socket) const {
        return handle() == socket.handle();
    }

    void socket::update_after_error(int code, bool disconnect) {
        if (disconnect && m_connected && (handle() == netsock::invalid_socket || (code != socket_error::WOULDBLOCK && code != socket_error::NOBUFS && code != socket_error::TIMEDOUT)))
            m_connected = false;
    }

    socket::~socket() {
        close();
    }
}