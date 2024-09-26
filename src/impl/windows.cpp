#include "netsock/impl.hpp"
#include <winsock2.h>
#include <ws2ipdef.h>
#include <ws2tcpip.h>
#include "netsock/endian.hpp"

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
                return SO_PROTOCOL_INFO;
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
                return SO_PROTOCOL_INFO;
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
        return  code == WSAEWOULDBLOCK ||
                code == WSAEINPROGRESS ||
                code == WSAEALREADY ||
                code == WSAENOBUFS ||
                code == WSAENOTCONN ||
                code == WSAEISCONN ||
                code == WSAESHUTDOWN ||
                code == WSAECONNRESET ||
                code == WSAECONNABORTED;
    }

    class windows_impl final : public impl {
        static constexpr WORD _ver = MAKEWORD(2, 2);
        int _code = 0;

        template <typename Exception>
        void _throw_and_set_code(Exception &&exception) {
            _code = WSAGetLastError();
            if (!is_safe(_code))
                throw exception;
            if (_code == WSAECONNRESET || _code == WSAECONNABORTED)
                throw socket_closed_error();
        }
    public:
        windows_impl() {
            WSAData data{};
            if (WSAStartup(_ver, &data) < 0)
                _throw_and_set_code(socket_error("WSAStartup"));
        }

        socket_t create_socket(const address_family family, const socket_type type, const ip_protocol protocol) override {
            return ::socket(map_af(family), map_sock(type), map_proto(protocol));
        }

        void close_socket(const socket_t socket) override {
            closesocket(socket);
        }

        void bind(const socket_t socket, const socket_address &address) override {
            if (::bind(socket, address.as<sockaddr>(), (int)address.size()) < 0)
                _throw_and_set_code(socket_error("bind"));
        }

        void listen(const socket_t socket, const int backlog) override {
            if (::listen(socket, backlog) < 0)
                _throw_and_set_code(socket_error("listen"));
        }

        socket_t accept(const socket_t socket, socket_address *out) override {
            if (out) {
                std::size_t len = sizeof(sockaddr_storage);
                *out = socket_address(len);
                const socket_t sock = ::accept(socket, out->as<sockaddr>(), (int *)&len);
                if (sock == INVALID_SOCKET)
                    _throw_and_set_code(socket_error("accept"));
                out->resize(len);
                return sock;
            }
            const socket_t sock = ::accept(socket, nullptr, nullptr);
            if (sock == INVALID_SOCKET)
                _throw_and_set_code(socket_error("accept"));
            return sock;
        }

        void connect(const socket_t socket, const socket_address &address) override {
            if (::connect(socket, address.as<sockaddr>(), (int)address.size()) < 0)
                _throw_and_set_code(socket_error("connect"));
        }

        std::size_t send(const socket_t socket, const std::byte *data, const std::size_t len) override {
            const int l = ::send(socket, (char const*)data, (int)len, 0);
            if (l < 0)
                _throw_and_set_code(socket_error("send"));
            return l;
        }

        std::size_t recv(const socket_t socket, std::byte *data, const std::size_t len) override {
            const int l = ::recv(socket, (char*)data, (int)len, 0);
            if (l < 0)
                _throw_and_set_code(socket_error("recv"));
            return l;
        }

        std::size_t sendto(const socket_t socket, const std::byte *data, const std::size_t len, const socket_address &to) override {
            const int l = ::sendto(socket, (char const*)data, (int)len, 0, to.as<sockaddr>(), (int)to.size());
            if (l < 0)
                _throw_and_set_code(socket_error("sendto"));
            return l;
        }

        std::size_t recvfrom(const socket_t socket, std::byte *data, const std::size_t len, socket_address &from) override {
            std::size_t size = sizeof(sockaddr_storage);
            from = socket_address(size);
            const int l = ::recvfrom(socket, (char*)data, (int)len, 0, from.as<sockaddr>(), (int *)&size);
            if (l < 0)
                _throw_and_set_code(socket_error("recvfrom"));
            from.resize(size);
            return l;
        }

        void set_option(const socket_t socket, const option::socket option, const void *value, const std::size_t len) override {
            if (setsockopt(socket, SOL_SOCKET, map_sock_option(option), (const char *)value, (int)len) < 0)
                _throw_and_set_code(socket_error("setsockopt"));
        }

        void get_option(const socket_t socket, const option::socket option, void *value, std::size_t &len) override {
            if (option == option::so_domain) {
                WSAPROTOCOL_INFO info;
                int size = sizeof(info);
                if (getsockopt(socket, SOL_SOCKET, map_sock_option(option), (char *)&info, &size) < 0)
                    _throw_and_set_code(socket_error("getsockopt"));
                if (len < sizeof(int))
                    throw std::invalid_argument("value size too small");
                *(int *)value = info.iAddressFamily;
                len = sizeof(int);
            }
            if (option == option::so_protocol) {
                WSAPROTOCOL_INFO info;
                int size = sizeof(info);
                if (getsockopt(socket, SOL_SOCKET, map_sock_option(option), (char *)&info, &size) < 0)
                    _throw_and_set_code(socket_error("getsockopt"));
                if (len < sizeof(int))
                    throw std::invalid_argument("value size too small");
                *(int *)value = info.iProtocol;
                len = sizeof(int);
            }
            if (getsockopt(socket, SOL_SOCKET, map_sock_option(option), (char *)value, (int *)&len) < 0)
                _throw_and_set_code(socket_error("getsockopt"));
        }

        void set_option(const socket_t socket, const option::tcp option, const void *value, const std::size_t len) override {
            if (setsockopt(socket, IPPROTO_TCP, map_tcp_option(option), (const char *)value, (int)len) < 0)
                _throw_and_set_code(socket_error("setsockopt"));
        }

        void get_option(const socket_t socket, const option::tcp option, void *value, std::size_t &len) override {
            if (getsockopt(socket, IPPROTO_TCP, map_tcp_option(option), (char *)value, (int *)&len))
                _throw_and_set_code(socket_error("getsockopt"));
        }

        void parse_address(const std::string &address, addrv4_t &out) override {
            addrv4_t tmp;
            if (inet_pton(AF_INET, address.c_str(), &tmp) == 0)
                throw std::invalid_argument("invalid IPv4 address");
            out = network_to_host(tmp);
        }

        void parse_address(const std::string &address, addrv6_t &out) override {
            addrv6_t tmp;
            if (inet_pton(AF_INET6, address.c_str(), &tmp) == 0)
                throw std::invalid_argument("invalid IPv6 address");
            network_to_host.copy(out, tmp, 16);
        }

        socket_address get_socket_address(const socket_t socket) override {
            std::size_t len = sizeof(sockaddr_storage);
            socket_address address(len);
            if (getsockname(socket, address.as<sockaddr>(), (int *)&len) < 0)
                _throw_and_set_code(socket_error("getsockname"));
            address.resize(len);
            return address;
        }

        ip_endpoint convert_address(const socket_address &address) override {
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

        socket_address convert_address(const ip_endpoint &address) override {
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

        std::size_t poll(poll_list &list, const std::chrono::milliseconds timeout) override {
            std::vector<pollfd> fds(list.size());
            for (std::size_t i = 0; i < list.size(); i++) {
                fds[i].fd = list[i].socket->handle();
                fds[i].events = map_poll_flags(list[i].events);
            }
            const int res = WSAPoll(fds.data(), list.size(), (int)timeout.count());
            if (res < 0)
                _throw_and_set_code(socket_error("poll"));
            for (std::size_t i = 0; i < list.size(); i++) {
                list[i].result = map_poll_events(fds[i].revents);
            }
            return res;
        }

        int poll(const socket_t socket, const int events, const std::chrono::milliseconds timeout) override {
            pollfd fd{socket, map_poll_flags(events), 0};
            if (WSAPoll(&fd, 1, (int)timeout.count()) < 0)
                _throw_and_set_code(socket_error("poll"));
            return fd.revents;
        }

        int error_code() override {
            return _code;
        }

        ~windows_impl() override {
            WSACleanup();
        }
    };

    impl_initializer<windows_impl> _init{};
}