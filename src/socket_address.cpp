#include "netsock/socket_address.hpp"

#include <algorithm>
#include <stdexcept>

namespace netsock {
    socket_address::socket_address(): _ptr(nullptr), _size(0) {

    }

    socket_address::socket_address(const std::size_t size): _ptr(new char[size]{}), _size(size) {

    }

    socket_address::socket_address(const address_family family, const std::size_t size): _ptr(nullptr), _size(size) {
        if (size < sizeof(address_family))
            throw std::invalid_argument("invalid address size");
        _ptr = new char[size];
        *(address_family *)_ptr = family;
    }

    socket_address::socket_address(const socket_address &address): _ptr(new char[address.size()]{}), _size(address.size()) {
        std::copy_n(address._ptr, address.size(), _ptr);
    }

    socket_address::socket_address(socket_address &&address) noexcept: _ptr(address.data()), _size(address.size()) {
        address._ptr = nullptr;
        address._size = 0;
    }

    socket_address &socket_address::operator=(const socket_address &address) {
        if (this == &address)
            return *this;
        delete[] _ptr;
        _ptr = new char[address.size()];
        _size = address.size();
        std::copy_n(address._ptr, address.size(), _ptr);
        return *this;
    }

    socket_address &socket_address::operator=(socket_address &&address) noexcept {
        delete[] _ptr;
        _ptr = address.data();
        _size = address.size();
        address._ptr = nullptr;
        address._size = 0;
        return *this;
    }

    char *socket_address::data() {
        return _ptr;
    }

    const char *socket_address::data() const {
        return _ptr;
    }

    std::size_t socket_address::size() const {
        return _size;
    }

    void socket_address::resize(const std::size_t size) {
        if (_size < size) {
            auto *tmp = new char[size];
            std::copy_n(_ptr, _size, tmp);
            delete[] _ptr;
            _ptr = tmp;
        }
        _size = size;
    }

}
