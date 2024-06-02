#ifndef NETSOCK_BASE_HPP
#define NETSOCK_BASE_HPP

#include <cstdint>
#include <cstddef>


struct sockaddr;
struct fd_set;
struct timeval;
struct addrinfo;

namespace netsock::impl {
    typedef uint64_t socket_t;
    typedef int64_t result_t;
    typedef int32_t addr_family_t;
    typedef int32_t sock_type_t;
    typedef int32_t ip_proto_t;
    typedef int32_t socklen_t;

    extern int32_t msg_nosignal;

    static const socket_t invalid_socket = socket_t(-1);

    socket_t socket(addr_family_t family, sock_type_t type, ip_proto_t proto);
    result_t connect(socket_t socket, const sockaddr *addr, socklen_t size);
    result_t select(socket_t nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, const timeval *timeout);
    result_t getsockopt(socket_t socket, int32_t level, int32_t optname, void *val, size_t *len);
    result_t setsockopt(socket_t socket, int32_t level, int32_t optname, const void *val, size_t len);
    result_t shutdown(socket_t socket, int32_t how);
    result_t close(socket_t socket);
    result_t getsockname(socket_t socket, sockaddr *name, socklen_t *len);
    socket_t accept(socket_t socket, sockaddr *addr, socklen_t *len);
    result_t bind(socket_t socket, const sockaddr *address, socklen_t len);
    result_t listen(socket_t socket, int32_t backlog);
    result_t recv(socket_t socket, void *buf, size_t len, int32_t flags);
    result_t send(socket_t socket, const void *buf, size_t len, int32_t flags);
    result_t recvfrom(socket_t socket, void *buf, size_t len, int32_t flags, sockaddr *addr, socklen_t *addrlen);
    result_t sendto(socket_t socket, const void *buf, size_t len, int32_t flags, const sockaddr *addr, socklen_t addrlen);
    result_t inet_pton(addr_family_t family, const char *p, void *addr);
    result_t getaddrinfo(const char *nodename, const char *servername, const addrinfo *hints, addrinfo **res);
}


#endif //NETSOCK_BASE_HPP
