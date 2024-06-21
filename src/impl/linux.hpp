#ifndef NETSOCK_LINUX_HPP
#define NETSOCK_LINUX_HPP
#include <cstddef>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include "netsock/impl/base.hpp"

namespace netsock::impl {
    int32_t msg_nosignal = MSG_NOSIGNAL;

    socket_t socket(addr_family_t family, sock_type_t type, ip_proto_t proto) {
        return ::socket(family, type, proto);
    }

    result_t connect(socket_t socket, const sockaddr *addr, socklen_t size) {
        return ::connect((int32_t)socket, addr, (int)size);
    }

    result_t select(socket_t nfds, void *readfds, void *writefds, void *exceptfds, timeval *timeout) {
        return ::select((int32_t)nfds, (fd_set *)readfds, (fd_set *)writefds, (fd_set *)exceptfds, timeout);
    }

    result_t getsockopt(socket_t socket, int32_t level, int32_t optname, void *val, size_t *len) {
        return ::getsockopt((int32_t)socket, level, optname, (char *)val, (uint32_t *)len);
    }

    result_t setsockopt(socket_t socket, int32_t level, int32_t optname, const void *val, size_t len) {
        return ::setsockopt((int32_t)socket, level, optname, (const char *)val, (uint32_t)len);
    }

    result_t shutdown(socket_t socket, int32_t how) {
        return ::shutdown((int32_t)socket, how);
    }

    result_t close(socket_t socket) {
        return ::close((int32_t)socket);
    }

    result_t getsockname(socket_t socket, sockaddr *name, socklen_t *len) {
        return ::getsockname((int32_t)socket, name, (uint32_t *)len);
    }

    socket_t accept(socket_t socket, sockaddr *addr, socklen_t *len) {
        return ::accept((int32_t)socket, addr, (uint32_t *)len);
    }

    result_t bind(socket_t socket, const sockaddr *address, socklen_t len) {
        return ::bind((int32_t)socket, address, len);
    }

    result_t listen(socket_t socket, int32_t backlog) {
        return ::listen((int32_t)socket, backlog);
    }

    result_t recv(socket_t socket, void *buf, size_t len, int32_t flags) {
        return ::recv((int32_t)socket, (char *)buf, (int)len, flags);
    }

    result_t send(socket_t socket, const void *buf, size_t len, int32_t flags) {
        return ::send((int32_t)socket, (const char *)buf, (int)len, flags);
    }

    result_t recvfrom(socket_t socket, void *buf, size_t len, int32_t flags, sockaddr *addr, socklen_t *addrlen) {
        return ::recvfrom((int32_t)socket, (char *)buf, (int)len, flags, addr, (uint32_t *)addrlen);
    }

    result_t sendto(socket_t socket, const void *buf, size_t len, int32_t flags, const sockaddr *addr, socklen_t addrlen) {
        return ::sendto((int32_t)socket, (const char *)buf, (int)len, flags, addr, addrlen);
    }

    result_t inet_pton(addr_family_t family, const char *p, void *addr) {
        return ::inet_pton(family, p, addr);
    }

    result_t getaddrinfo(const char *nodename, const char *servername, const addrinfo *hints, addrinfo **res) {
        return ::getaddrinfo(nodename, servername, hints, res);
    }
}

#endif //NETSOCK_LINUX_HPP
