#ifndef SOCKET_ADDRESS_H
#define SOCKET_ADDRESS_H

#include <string>
#include "types.hpp"

#include "enum.hpp"

namespace netsock {
    class socket_address {
    public:
        socket_address();

        explicit socket_address(std::size_t size);
        socket_address(address_family family, std::size_t size);
        socket_address(const socket_address &address);
        socket_address &operator=(const socket_address &address);
        socket_address(socket_address &&address) noexcept;
        socket_address &operator=(socket_address &&address) noexcept;

        template <typename T>
        [[nodiscard]] const T *as() const {
            return (const T *)data();
        }

        template <typename T>
        [[nodiscard]] T *as() {
            return (T *)data();
        }

        [[nodiscard]] const char *data() const;
        [[nodiscard]] char *data();
        [[nodiscard]] std::size_t size() const;

        void resize(std::size_t size);
    private:
        char *_ptr;
        std::size_t _size;
    };
}

#endif //SOCKET_ADDRESS_H
