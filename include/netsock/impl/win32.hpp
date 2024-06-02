#ifndef NETSOCK_WIN32_HPP
#define NETSOCK_WIN32_HPP

#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstddef>

#include "netsock/impl/base.hpp"

namespace netsock::impl {
    int32_t msg_nosignal = 0;

    socket_t socket(addr_family_t family, sock_type_t type, ip_proto_t proto) {
        return ::socket(family, type, proto);
    }

    result_t connect(socket_t socket, const sockaddr *addr, socklen_t size) {
        return ::connect(socket, addr, (int)size);
    }

    result_t select(socket_t nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, const timeval *timeout) {
        return ::select((int)nfds, readfds, writefds, exceptfds, timeout);
    }

    result_t getsockopt(socket_t socket, int32_t level, int32_t optname, void *val, size_t *len) {
        return ::getsockopt(socket, level, optname, (char *)val, (int *)len);
    }

    result_t setsockopt(socket_t socket, int32_t level, int32_t optname, const void *val, size_t len) {
        return ::setsockopt(socket, level, optname, (const char *)val, (int)len);
    }

    result_t shutdown(socket_t socket, int32_t how) {
        return ::shutdown(socket, how);
    }

    result_t close(socket_t socket) {
        return ::closesocket(socket);
    }

    result_t getsockname(socket_t socket, sockaddr *name, socklen_t *len) {
        return ::getsockname(socket, name, len);
    }

    socket_t accept(socket_t socket, sockaddr *addr, socklen_t *len) {
        return ::accept(socket, addr, len);
    }

    result_t bind(socket_t socket, const sockaddr *address, socklen_t len) {
        return ::bind(socket, address, len);
    }

    result_t listen(socket_t socket, int32_t backlog) {
        return ::listen(socket, backlog);
    }

    result_t recv(socket_t socket, void *buf, size_t len, int32_t flags) {
        return ::recv(socket, (char *)buf, (int)len, flags);
    }

    result_t send(socket_t socket, const void *buf, size_t len, int32_t flags) {
        return ::send(socket, (const char *)buf, (int)len, flags);
    }

    result_t recvfrom(socket_t socket, void *buf, size_t len, int32_t flags, sockaddr *addr, socklen_t *addrlen) {
        return ::recvfrom(socket, (char *)buf, (int)len, flags, addr, addrlen);
    }

    result_t sendto(socket_t socket, const void *buf, size_t len, int32_t flags, const sockaddr *addr, socklen_t addrlen) {
        return ::sendto(socket, (const char *)buf, (int)len, flags, addr, addrlen);
    }

    result_t inet_pton(addr_family_t family, const char *p, void *addr) {
        return ::inet_pton(family, p, addr);
    }
}

#endif //NETSOCK_WIN32_HPP
