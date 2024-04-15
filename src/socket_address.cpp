#include "netsock/socket_address.hpp"
#include <algorithm>
#if defined(__WIN32__) || defined(__WIN64__)
#include <ws2ipdef.h>
#endif

namespace netsock {
    socket_address::socket_address(netsock::address_family family, size_t size) {
        m_data = new char[size];
        std::fill(m_data, m_data + size, '\0');
        m_bufferSize = size;
        auto *addr = (sockaddr *)m_data;
        addr->sa_family = family;
    }

    socket_address &socket_address::operator=(netsock::socket_address &&address) noexcept{
        delete[] m_data;
        m_data = address.m_data;
        m_bufferSize = address.m_bufferSize;
        address.m_data = nullptr;
        return *this;
    }

    socket_address::socket_address(netsock::socket_address &&address) noexcept{
        m_data = address.m_data;
        m_bufferSize = address.m_bufferSize;
        address.m_data = nullptr;
    }

    socket_address::socket_address(address_family family): socket_address(family, family == inet ? sizeof(sockaddr_in) : sizeof(sockaddr_in6)){

    }

    const char *socket_address::data() const {
        return m_data;
    }

    char *socket_address::data() {
        return m_data;
    }

    size_t socket_address::size() const {
        return m_bufferSize;
    }

    address_family socket_address::family() const {
        return (address_family)((sockaddr *)m_data)->sa_family;
    }

    socket_address::~socket_address(){
        delete[] m_data;
    }
}