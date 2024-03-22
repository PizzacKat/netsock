#ifndef NETSOCK_SOCKET_ADDRESS_HPP
#define NETSOCK_SOCKET_ADDRESS_HPP

#include <cstddef>
#include "types.hpp"

namespace netsock {
    class socket_address {
    public:
        socket_address(address_family family, size_t size);
        explicit socket_address(address_family family);
        socket_address(const socket_address &)= delete;
        socket_address(socket_address &&address) noexcept;
        socket_address &operator=(const socket_address &)= delete;
        socket_address &operator=(socket_address &&address) noexcept;

        [[nodiscard]] const char *data() const;
        char *data();
        [[nodiscard]] size_t size() const;
        [[nodiscard]] address_family family() const;

        ~socket_address();
    private:
        char *m_data;
        size_t m_bufferSize;
    };
}

#endif //NETSOCK_SOCKET_ADDRESS_HPP
