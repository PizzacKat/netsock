#ifndef POLL_LIST_HPP
#define POLL_LIST_HPP

#include "socket.hpp"

namespace netsock {
    enum poll_flag {
        pollin = 0x1, pollout = 0x2, pollerr = 0x4
    };

    struct poll_entry {
        socket *socket;
        int events;
        int result;

        poll_entry(class socket &socket, const int events): socket(&socket), events(events), result(0) {

        }
    };

    class poll_list {
    public:
        poll_list();
        poll_list(std::initializer_list<poll_entry> entries);

        void push(const poll_entry &entry);
        void push(socket &socket, int events);
        void remove(const socket &socket);

        std::size_t poll(std::chrono::milliseconds timeout = poll_wait);

        [[nodiscard]] std::size_t size() const;

        poll_entry &operator[](std::size_t index);

        [[nodiscard]] int result(const socket &socket) const;
    private:
        std::vector<poll_entry> _entries;
    };
}

#endif //POLL_LIST_HPP
