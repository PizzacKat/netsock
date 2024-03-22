#ifndef NETSOCK_TYPES_HPP
#define NETSOCK_TYPES_HPP

#include <cstdint>

#if defined(__WIN32__) || defined(__WIN64__)
#include <winsock2.h>
#include <wsrm.h>
#include <ws2bth.h>
#include <mswsock.h>
#include <wsnwlink.h>
#include <ws2ipdef.h>
#elif defined(__unix__)
#include <asm-generic/socket.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <netinet/in.h>

#endif

namespace netsock {
    enum poll_mode {
        read, write, error
    };

    namespace option {
        enum socket {
            broadcast = SO_BROADCAST,
            debug = SO_DEBUG,
            dont_route = SO_DONTROUTE,
            keep_alive = SO_KEEPALIVE,
            linger = SO_LINGER,
            oob_inline = SO_OOBINLINE,
            receive_buffer = SO_RCVBUF,
            receive_timeout = SO_RCVTIMEO,
            send_buffer = SO_SNDBUF,
            send_timeout = SO_SNDTIMEO,
            type = SO_TYPE,
            accept_connection = SO_ACCEPTCONN,
            error = SO_ERROR,
            reuse_address = SO_REUSEADDR,
#if defined(__WIN32__) || defined(__WIN64__)
            port_scalability = 0x3006,
            bsp_state = 0x1009,
            conditional_accept = SO_CONDITIONAL_ACCEPT,
            dont_linger = SO_DONTLINGER,
            group_priority = SO_GROUP_PRIORITY,
            exclusive_address = SO_EXCLUSIVEADDRUSE,
            update_accept_context = SO_UPDATE_ACCEPT_CONTEXT,
            config = PVD_CONFIG,
            connection_time = SO_CONNECT_TIME,
            max_message_size = SO_MAX_MSG_SIZE,
            protocol_info = SO_PROTOCOL_INFO
#elif defined(__unix__)
            bind_to_device = SO_BINDTODEVICE,
            bsd_compatibility = SO_BSDCOMPAT,
            domain = SO_DOMAIN,
            mark = SO_MARK,
            pass_credentials = SO_PASSCRED,
            peer_credentials = SO_PEERCRED,
            priority = SO_PRIORITY,
            protocol = SO_PROTOCOL,
            receive_low_at = SO_RCVLOWAT,
            send_low_at = SO_SNDLOWAT,
            send_buffer_force = SO_SNDBUFFORCE,
            receive_buffer_force = SO_RCVBUFFORCE,
            timestamp = SO_TIMESTAMP
#endif
        };

        enum tcp {
            no_delay = TCP_NODELAY
        };

        enum ipv6 {
            hop_limit = IPV6_HOPLIMIT,
            ipv6_only = IPV6_V6ONLY,
#if defined(__WIN64__) || defined(__WIN32__)
            ip_protection_level = IPV6_PROTECTION_LEVEL
#endif
        };
    }

    enum address_family {
        unspec = AF_UNSPEC,
        inet = AF_INET,
        inet6 = AF_INET6
    };

    enum socket_type {
        stream = SOCK_STREAM,
        dgram = SOCK_DGRAM,
        raw = SOCK_RAW,
        rdm = SOCK_RDM,
        seqpacket = SOCK_SEQPACKET
    };

    enum ip_protocol {
        icmp = IPPROTO_ICMP,
        igmp = IPPROTO_IGMP,
        tcp = IPPROTO_TCP,
        udp = IPPROTO_UDP,
        icmpv6 = IPPROTO_ICMPV6,
#if defined(__WIN32__) || defined(__WIN64__)
        rfcomm = BTHPROTO_RFCOMM,
        rm = IPPROTO_RM
#endif
    };

#if defined(__WIN32__) || defined(__WIN64__)
    typedef uint64_t socket_t;
#elif defined(__unix__)
    typedef int socket_t;
#endif
    inline socket_t invalid_socket = ~(socket_t)0;
}

#endif //NETSOCK_TYPES_HPP
