#include <netsock/select_list.hpp>
#include <netsock/socket/socket.hpp>
#include <algorithm>

namespace netsock {
    select_list::select_list()= default;

    void select_list::add(const socket &socket) {
        m_sockets.push_back(&socket);
    }

    bool select_list::has(const socket &socket) const {
        return std::find(m_sockets.begin(), m_sockets.end(), &socket) != m_sockets.end();
    }

    bool select_list::empty() const {
        return m_sockets.empty();
    }

    size_t select_list::size() const {
        return m_sockets.size();
    }

    const std::vector<const socket *> &select_list::sockets() const {
        return m_sockets;
    }

    std::vector<const socket *> &select_list::sockets() {
        return m_sockets;
    }

    socket_t select_list::max() const {
        socket_t max = 0;
        for (const socket *sock : m_sockets)
            max = max < sock->handle() ? sock->handle() : max;
        return max;
    }
}