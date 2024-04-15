#ifndef NETSOCK_SELECT_LIST_HPP
#define NETSOCK_SELECT_LIST_HPP

#include <vector>
#include "netsock/types.hpp"

namespace netsock {
    class socket;
    class select_list {
    public:
        select_list();
        void add(const socket &socket);

        [[nodiscard]] bool has(const socket &socket) const;
        [[nodiscard]] bool empty() const;
        [[nodiscard]] size_t size() const;

        [[nodiscard]] const std::vector<const socket *> &sockets() const;
        std::vector<const socket *> &sockets();
        [[nodiscard]] socket_t max() const;
    private:
        std::vector<const socket *> m_sockets;
    };
}

#endif //NETSOCK_SELECT_LIST_HPP
