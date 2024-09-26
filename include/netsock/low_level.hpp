#ifndef LOW_LEVEL_HPP
#define LOW_LEVEL_HPP

#include "impl.hpp"
#include "socket.hpp"

namespace netsock::low_level {
    inline void set_option(const socket &socket, const option::socket option, const void *value, const std::size_t length) {
        impl::impl->set_option(socket.handle(), option, value, length);
    }

    inline void set_option(const socket &socket, const option::tcp option, const void *value, const std::size_t length) {
        impl::impl->set_option(socket.handle(), option, value, length);
    }

    inline void get_option(const socket &socket, const option::socket option, void *value, std::size_t &length) {
        impl::impl->get_option(socket.handle(), option, value, length);
    }

    inline void get_option(const socket &socket, const option::tcp option, void *value, std::size_t &length) {
        impl::impl->get_option(socket.handle(), option, value, length);
    }
}

#endif //LOW_LEVEL_HPP
