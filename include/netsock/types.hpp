#ifndef TYPES_HPP
#define TYPES_HPP
#include <chrono>
#include <cstddef>
#include <stdexcept>
#include <variant>

#include "enum.hpp"

namespace netsock {
    // typedef uint8_t byte;
    typedef std::size_t socket_t;
    typedef std::size_t socklen_t;
    typedef unsigned int addrv4_t;
    typedef unsigned short addrv6_t[8];

    static constexpr std::chrono::milliseconds poll_wait{-1};
    static constexpr std::chrono::milliseconds poll_nowait{0};

    class socket_error : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    class socket_closed_error final : public std::exception {

    };

    // namespace options {
    //     namespace socket {
    //         template <option::socket Value>
    //         struct enum_value { static constexpr auto value = Value; };
    //
    //         struct accepts_connections : enum_value<option::so_acceptconn> { bool accepts; };
    //         struct bind_to_device : enum_value<option::so_bindtodevice> { std::string device; };
    //         struct broadcast : enum_value<option::so_broadcast> { bool enable; };
    //         struct debug : enum_value<option::so_debug> { bool enable; };
    //         struct domain : enum_value<option::so_domain> { address_family value; };
    //         struct error : enum_value<option::so_error> { int32_t value; };
    //         struct dont_linger : enum_value<option::so_dontlinger> { bool active; };
    //         struct dont_route : enum_value<option::so_dontroute> { bool enable; };
    //         struct keep_alive : enum_value<option::so_keepalive> { bool enable; };
    //         struct linger : enum_value<option::so_linger> { int active; int seconds; };
    //         struct mark : enum_value<option::so_mark> { bool value; };
    //         struct max_message_size : enum_value<option::so_maxmsgsize> { int32_t size; };
    //         struct oob_inline : enum_value<option::so_oobinline> { bool value; };
    //         struct priority : enum_value<option::so_priority> { int value; };
    //         struct protocol : enum_value<option::so_protocol> { ip_protocol value; };
    //         struct receive_buffer : enum_value<option::so_rcvbuf> { int size; };
    //         struct send_buffer : enum_value<option::so_sndbuf> { int size; };
    //         struct receive_low_at : enum_value<option::so_rcvlowat> { int at; };
    //         struct send_low_at : enum_value<option::so_sndlowat> { int at; };
    //         struct receive_timeout : enum_value<option::so_rcvtimeo> { std::chrono::milliseconds timeout; };
    //         struct send_timeout : enum_value<option::so_sndtimeo> { std::chrono::milliseconds timeout; };
    //         struct reuse_address : enum_value<option::so_reuseaddr> { bool enable; };
    //         struct reuse_port : enum_value<option::so_reuseport> { bool enable; };
    //         struct timestamp : enum_value<option::so_timestamp> { bool enable; };
    //         struct timestampns : enum_value<option::so_timestampns> { bool enable; };
    //         struct type : enum_value<option::so_type> { socket_type value; };
    //
    //         typedef std::variant<accepts_connections,
    //                              bind_to_device,
    //                              broadcast,
    //                              debug,
    //                              domain,
    //                              error,
    //                              dont_linger,
    //                              dont_route,
    //                              keep_alive,
    //                              linger,
    //                              mark,
    //                              max_message_size,
    //                              oob_inline,
    //                              priority,
    //                              protocol,
    //                              receive_buffer,
    //                              send_buffer,
    //                              receive_low_at,
    //                              send_low_at,
    //                              receive_timeout,
    //                              send_timeout,
    //                              reuse_address,
    //                              reuse_port,
    //                              timestamp,
    //                              timestampns,
    //                              type> variant;
    //     }
    //
    //     namespace tcp {
    //         template <option::tcp Value>
    //         struct enum_value { static constexpr auto value = Value; };
    //
    //         struct cork : enum_value<option::tcp_cork> { bool enable; };
    //         struct no_delay : enum_value<option::tcp_nodelay> { bool enable; };
    //         struct max_segmentation : enum_value<option::tcp_maxseg> { int size; };
    //         struct keep_count : enum_value<option::tcp_keepcnt> { int value; };
    //         struct keep_idle : enum_value<option::tcp_keepidle> { std::chrono::seconds timeout; };
    //         struct keep_interval : enum_value<option::tcp_keepintvl> { std::chrono::seconds interval; };
    //         struct quick_ack : enum_value<option::tcp_quickack> { bool enable; };
    //         struct syn_count : enum_value<option::tcp_syncnt> { int value; };
    //
    //         typedef std::variant<cork,
    //                              no_delay,
    //                              max_segmentation,
    //                              keep_count,
    //                              keep_idle,
    //                              keep_interval,
    //                              quick_ack,
    //                              syn_count> variant;
    //     }
    //
    //     namespace udp {
    //         template <option::udp Value>
    //         struct enum_value { static constexpr auto value = Value; };
    //
    //         struct cork : enum_value<option::udp_cork> { bool enable; };
    //         struct receive_max_coalesced_size : enum_value<option::udp_recvmaxcoalescedsize> { int value; };
    //         struct send_message_size : enum_value<option::udp_sendmsgsize> { int enable; };
    //     }
    // }
}

#endif //TYPES_HPP
