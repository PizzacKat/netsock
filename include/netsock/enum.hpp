#ifndef ENUM_H
#define ENUM_H

namespace netsock {
    enum address_family {
        af_inet,
        af_inet6,
        af_unspec,
        af_packet,
        af_local
    };

    enum socket_type {
        sock_stream,
        sock_dgram,
        sock_raw,
        sock_seqpacket,
        sock_rdm,
        sock_packet
    };

    enum ip_protocol {
        ipproto_ip,
        ipproto_tcp,
        ipproto_udp,
        ipproto_raw
    };

    namespace option {
        enum socket {
            so_acceptconn,
            so_bindtodevice,
            so_broadcast,
            so_debug,
            so_domain,
            so_error,
            so_dontlinger,
            so_dontroute,
            so_keepalive,
            so_linger,
            so_mark,
            so_maxmsgsize,
            so_oobinline,
            so_passcred,
            so_peercred,
            so_priority,
            so_protocol,
            so_rcvbuf,
            so_rcvlowat,
            so_rcvtimeo,
            so_reuseaddr,
            so_reuseport,
            so_sndbuf,
            so_sndlowat,
            so_sndtimeo,
            so_timestamp,
            so_timestampns,
            so_type
        };

        enum tcp {
            tcp_cork,
            tcp_nodelay,
            tcp_maxseg,
            tcp_keepcnt,
            tcp_keepidle,
            tcp_keepintvl,
            tcp_quickack,
            tcp_syncnt,
        };

        enum udp {
            udp_cork,
            udp_recvmaxcoalescedsize,
            udp_sendmsgsize
        };
    }

    // enum error_code {
    //     eintr,
    //     eacces,
    //     efault,
    //     einval,
    //     emfile,
    //     ewouldblock,
    //     einprogress,
    //     ealready,
    //     enotsock,
    //     edestaddrreq,
    //     emsgsize,
    //     eprototype,
    //     enoprotoopt,
    //     eprotonosupport,
    //     esocktnosupport,
    //     eopnotsupp,
    //     epfnosupport,
    //     eafnosupport,
    //     eaddrinuse,
    //     eaddrnotavail,
    //     enetdown,
    //     enetunreach,
    //     enetreset,
    //     econnaborted,
    //     econnreset,
    //     enobufs,
    //     eisconn,
    //     enotconn,
    //     etimedout,
    //     econnrefused,
    //     ehostunreach
    // };
}

#endif //ENUM_H
