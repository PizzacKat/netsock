#include "netsock/impl.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <poll.h>
#include "netsock/endian.hpp"
#include <fcntl.h>

namespace netsock::impl {
    static int map_af(const address_family family) {
        switch (family) {
            case af_inet:
                return AF_INET;
            case af_inet6:
                return AF_INET6;
            case af_unspec:
                return AF_UNSPEC;
            default:
                break;
        }
        throw std::invalid_argument("unsupported address family");
    }

    static int map_sock(const socket_type type) {
        switch (type) {
            case sock_stream:
                return SOCK_STREAM;
            case sock_dgram:
                return SOCK_DGRAM;
            case sock_raw:
                return SOCK_RAW;
            case sock_seqpacket:
                return SOCK_SEQPACKET;
            case sock_rdm:
                return SOCK_RDM;
            default:
                break;
        }
        throw std::invalid_argument("unsupported socket type");
    }

    static int map_proto(const ip_protocol protocol) {
        switch (protocol) {
            case ipproto_ip:
                return IPPROTO_IP;
            case ipproto_tcp:
                return IPPROTO_TCP;
            case ipproto_udp:
                return IPPROTO_UDP;
            case ipproto_raw:
                return IPPROTO_RAW;
            default:
                break;
        }
        throw std::invalid_argument("unsupported socket type");
    }

    static int map_sock_option(const option::socket option) {
        switch (option) {
            case option::so_acceptconn:
                return SO_ACCEPTCONN;
            case option::so_broadcast:
                return SO_BROADCAST;
            case option::so_debug:
                return SO_DEBUG;
            case option::so_domain:
                return SO_DOMAIN;
            case option::so_error:
                return SO_ERROR;
            case option::so_dontroute:
                return SO_DONTROUTE;
            case option::so_keepalive:
                return SO_KEEPALIVE;
            case option::so_linger:
                return SO_LINGER;
            case option::so_oobinline:
                return SO_OOBINLINE;
            case option::so_protocol:
                return SO_PROTOCOL;
            case option::so_rcvbuf:
                return SO_RCVBUF;
            case option::so_rcvlowat:
                return SO_RCVLOWAT;
            case option::so_rcvtimeo:
                return SO_RCVTIMEO;
            case option::so_reuseaddr:
                return SO_REUSEADDR;
            case option::so_sndbuf:
                return SO_SNDBUF;
            case option::so_sndlowat:
                return SO_SNDLOWAT;
            case option::so_sndtimeo:
                return SO_SNDTIMEO;
            case option::so_type:
                return SO_TYPE;
            default:
                throw std::invalid_argument("unsupported socket option");
        }
    }

    static int map_tcp_option(const option::tcp option) {
        switch (option) {
            case option::tcp_nodelay:
                return TCP_NODELAY;
            default:
                throw std::invalid_argument("unsupported tcp option");
        }
    }

    static short map_poll_flags(const int flags) {
        return (short)((flags & pollin ? POLLIN : 0) | (flags & pollout ? POLLOUT : 0) | (flags & pollerr ? POLLERR : 0));
    }

    static int map_poll_events(const short events) {
        return (events & POLLIN ? pollin : 0) | (events & POLLOUT ? pollout : 0) | (events & POLLERR ? pollerr : 0);
    }

    static bool is_safe(const int code) {
        return  code == EWOULDBLOCK ||
                code == EINPROGRESS ||
                code == EALREADY ||
                code == ENOBUFS ||
                code == ENOTCONN ||
                code == EISCONN ||
                code == ESHUTDOWN ||
                code == ECONNRESET ||
                code == ECONNABORTED;
    }

    int _code = 0;

    template <typename Exception>
    void _throw_and_set_code(Exception &&exception) {
        _code = errno;
        if (!is_safe(_code))
            throw exception;
        if (_code == ECONNRESET || _code == ECONNABORTED)
            throw socket_closed_error();
    }

    socket_t create_socket(const address_family family, const socket_type type, const ip_protocol protocol) {
        return ::socket(map_af(family), map_sock(type), map_proto(protocol));
    }

    void close_socket(const socket_t socket) {
        close((int)socket);
    }

    void bind(const socket_t socket, const socket_address &address) {
        if (::bind((int)socket, address.as<sockaddr>(), (int)address.size()) < 0)
            _throw_and_set_code(socket_error("bind"));
    }

    void listen(const socket_t socket, const int backlog) {
        if (::listen((int)socket, backlog) < 0)
            _throw_and_set_code(socket_error("listen"));
    }

    socket_t accept(const socket_t socket, socket_address *out) {
        if (out) {
            std::size_t len = sizeof(sockaddr_storage);
            *out = socket_address(len);
            const int sock = ::accept((int)socket, out->as<sockaddr>(), (::socklen_t *)&len);
            if (sock < 0)
                _throw_and_set_code(socket_error("accept"));
            out->resize(len);
            return sock;
        }
        const int sock = ::accept((int)socket, nullptr, nullptr);
        if (sock < 0)
            _throw_and_set_code(socket_error("accept"));
        return sock;
    }

    void connect(const socket_t socket, const socket_address &address) {
        if (::connect((int)socket, address.as<sockaddr>(), (int)address.size()) < 0)
            _throw_and_set_code(socket_error("connect"));
    }

    std::size_t send(const socket_t socket, const std::byte *data, const std::size_t len) {
        const ssize_t l = ::send((int)socket, (char const*)data, (int)len, 0);
        if (l < 0)
            _throw_and_set_code(socket_error("send"));
        return l;
    }

    std::size_t recv(const socket_t socket, std::byte *data, const std::size_t len) {
        const ssize_t l = ::recv((int)socket, (char*)data, (int)len, 0);
        if (l < 0)
            _throw_and_set_code(socket_error("recv"));
        return l;
    }

    std::size_t sendto(const socket_t socket, const std::byte *data, const std::size_t len, const socket_address &to) {
        const ssize_t l = ::sendto((int)socket, data, len, 0, to.as<sockaddr>(), (int)to.size());
        if (l < 0)
            _throw_and_set_code(socket_error("sendto"));
        return l;
    }

    std::size_t recvfrom(const socket_t socket, std::byte *data, const std::size_t len, socket_address &from) {
        std::size_t size = sizeof(sockaddr_storage);
        from = socket_address(size);
        const ssize_t l = ::recvfrom((int)socket, data, len, 0, from.as<sockaddr>(), (::socklen_t *)&size);
        if (l < 0)
            _throw_and_set_code(socket_error("recvfrom"));
        from.resize(size);
        return l;
    }

    void set_option(const socket_t socket, const option::socket option, const void *value, const std::size_t len) {
        if (setsockopt((int)socket, SOL_SOCKET, map_sock_option(option), value, (int)len) < 0)
            _throw_and_set_code(socket_error("setsockopt"));
    }

    void get_option(const socket_t socket, const option::socket option, void *value, std::size_t &len) {
        if (getsockopt((int)socket, SOL_SOCKET, map_sock_option(option), value, (::socklen_t *)&len) < 0)
            _throw_and_set_code(socket_error("getsockopt"));
    }

    void set_option(const socket_t socket, const option::tcp option, const void *value, const std::size_t len) {
        if (setsockopt((int)socket, IPPROTO_TCP, map_tcp_option(option), value, (::socklen_t)len) < 0)
            _throw_and_set_code(socket_error("setsockopt"));
    }

    void get_option(const socket_t socket, const option::tcp option, void *value, std::size_t &len) {
        if (getsockopt((int)socket, IPPROTO_TCP, map_tcp_option(option), value, (::socklen_t *)&len))
            _throw_and_set_code(socket_error("getsockopt"));
    }

    void parse_address(const std::string &address, addrv4_t &out) {
        addrv4_t tmp;
        if (inet_pton(AF_INET, address.c_str(), &tmp) == 0)
            throw std::invalid_argument("invalid IPv4 address");
        out = network_to_host(tmp);
    }

    void parse_address(const std::string &address, addrv6_t &out) {
        addrv6_t tmp;
        if (inet_pton(AF_INET6, address.c_str(), &tmp) == 0)
            throw std::invalid_argument("invalid IPv6 address");
        network_to_host.copy(out, tmp, 16);
    }

    socket_address get_socket_address(const socket_t socket) {
        std::size_t len = sizeof(sockaddr_storage);
        socket_address address(len);
        if (getsockname((int)socket, address.as<sockaddr>(), (::socklen_t *)&len) < 0)
            _throw_and_set_code(socket_error("getsockname"));
        address.resize(len);
        return address;
    }

    std::vector<socket_address> resolve_hosts(const std::string &hostname) {
        std::vector<socket_address> addresses;
        if (hostname.empty())
            return addresses;
        addrinfo *ai = nullptr;
        if (getaddrinfo(hostname.c_str(), nullptr, nullptr, &ai) < 0)
            _throw_and_set_code(socket_error("getaddrinfo"));
        if (ai == nullptr)
            return addresses;
        const addrinfo *info = ai;
        while (info != nullptr) {
            socket_address address(ai->ai_addrlen);
            memcpy(address.data(), ai->ai_addr, ai->ai_addrlen);
            addresses.push_back(address);
            info = info->ai_next;
        }
        freeaddrinfo(ai);
        return addresses;
    }

    ip_endpoint convert_address(const socket_address &address) {
        const auto *saddr = address.as<sockaddr>();
        if (saddr->sa_family != AF_INET && saddr->sa_family != AF_INET6)
            throw std::invalid_argument("unsupported address family");
        if (saddr->sa_family == AF_INET6) {
            addrv6_t bytes;
            const auto *addr6 =  (const sockaddr_in6 *)saddr;
            network_to_host.copy(bytes, &addr6->sin6_addr, 16);
            return {ip_address(bytes, network_to_host((uint32_t)addr6->sin6_scope_id)), network_to_host(addr6->sin6_port)};
        }
        const auto *addr = (const sockaddr_in *)saddr;
        const addrv4_t bytes = network_to_host(*(uint32_t*)&addr->sin_addr);
        return {ip_address(bytes), network_to_host(addr->sin_port)};
    }

    socket_address convert_address(const ip_endpoint &address) {
        if (address.address().family() == af_inet6) {
            socket_address saddr(sizeof(sockaddr_in6));
            auto *addr6 = saddr.as<sockaddr_in6>();
            addr6->sin6_family = AF_INET6;
            addr6->sin6_port = host_to_network(address.port());
            addr6->sin6_scope_id = host_to_network(address.address().scope());
            host_to_network.copy(&addr6->sin6_addr, address.address().addr6(), 16);
            return saddr;
        }
        socket_address saddr(sizeof(sockaddr_in));
        auto *addr = saddr.as<sockaddr_in>();
        addr->sin_family = AF_INET;
        addr->sin_port = host_to_network(address.port());
        *(uint32_t *)&addr->sin_addr = host_to_network(address.address().addr());
        return saddr;
    }

    std::size_t poll(poll_list &list, const std::chrono::milliseconds timeout) {
        std::vector<pollfd> fds(list.size());
        for (std::size_t i = 0; i < list.size(); i++) {
            fds[i].fd = (int)list[i].socket->handle();
            fds[i].events = map_poll_flags(list[i].events);
        }
        const int res = poll(fds.data(), list.size(), (int)timeout.count());
        if (res < 0)
            _throw_and_set_code(socket_error("poll"));
        for (std::size_t i = 0; i < list.size(); i++)
            list[i].result = map_poll_events(fds[i].revents);
        return res;
    }

    int poll(const socket_t socket, const int events, const std::chrono::milliseconds timeout) {
        pollfd fd{(int)socket, map_poll_flags(events), 0};
        const int result = poll(&fd, 1, (int)timeout.count());
        if (result < 0)
            _throw_and_set_code(socket_error("poll"));
        if (result == 0)
            return 0;
        return fd.revents;
    }

    void set_blocking(const socket_t socket, const bool blocking) {
        int flags = fcntl(static_cast<int>(socket), F_GETFL, 0);
        if (flags == -1)
            _throw_and_set_code(socket_error("fcntl"));
        flags = blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK;
        if (fcntl(static_cast<int>(socket), F_SETFL, flags) < 0)
            _throw_and_set_code(socket_error("fcntl"));
    }

    int error_code() {
        return _code;
    }
}