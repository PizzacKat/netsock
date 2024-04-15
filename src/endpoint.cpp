#include <iostream>
#include "netsock/endpoint.hpp"
#include "netsock/errors.hpp"

namespace netsock {
    endpoint::endpoint(const endpoint &endpoint) {
        if (endpoint == nullptr)
            m_storage = nullptr;
        else
            m_storage = endpoint.m_storage->copy();
    }

    endpoint &endpoint::from_address(const socket_address &address) {
        if (!m_storage)
            throw invalid_operation("Endpoint is null");
        m_storage->from_address(address);
        return *this;
    }

    const std::type_info &endpoint::type() const {
        if (!m_storage)
            throw invalid_operation("Endpoint is null");
        return m_storage->type();
    }

    address_family endpoint::family() const {
        if (!m_storage)
            throw invalid_operation("Endpoint is null");
        return m_storage->family();
    }

    socket_address endpoint::serialize() const {
        if (!m_storage)
            throw invalid_operation("Endpoint is null");
        return m_storage->serialize();
    }

    bool endpoint::operator==(std::nullptr_t const &) const {
        return m_storage == nullptr;
    }
}