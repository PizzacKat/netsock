#if defined(__WIN32__) || defined(__WIN64__)
#include <ws2tcpip.h>
#define s6_addr16 u.Word
#elif defined(__unix__)
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include "netsock/ip_address.hpp"
#include "netsock/endian.hpp"
#include "netsock/errors.hpp"

namespace netsock {
    template <typename T>
    T hostToNetwork(const T &value){
        T res;
        if (endian::endianess() == endian::small)
            endian::smallToBig((char *)&res, (char *)&value, sizeof(T));
        else
            res = value;
        return res;
    }

    template <typename T>
    T networkToHost(const T &value){
        T res;
        if (endian::endianess() == endian::small)
            endian::bigToSmall((char *)&res, (char *)&value, sizeof(T));
        else
            res = value;
        return res;
    }

    ip_address::ip_address()= default;

    ip_address::ip_address(const std::string &address) {
        if (address.find(':') != std::string::npos) {
            m_family = inet6;
            in6_addr addr6{};
            int res = inet_pton(inet6, address.c_str(), &addr6);
            if (res == 0)
                throw address_exception("Given address is not a valid IPv6 address");
            if (res == -1)
                throw address_exception("Error in inet_pton");
            for (size_t i = 0; i < 8; i++)
                m_values[i] = host_to_network(addr6.s6_addr16[i]);
        }
        else {
            m_family = inet;
            in_addr addr{};
            int res = inet_pton(inet, address.c_str(), &addr);
            if (res == 0)
                throw address_exception("Given address is not a valid IPv4 address");
            if (res == -1)
                throw address_exception("Error in inet_pton");
            m_address = addr.s_addr;
        }
    }

    ip_address::ip_address(const std::vector<unsigned char> &bytes) {
        if (bytes.size() == 4) {
            m_family = inet;
            m_address = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
            return;
        }
        if (bytes.size() == 16){
            m_family = inet6;
            for (int i = 0; i < 8; i++)
                m_values[i] = (bytes[i * 2] << 8) | bytes[i * 2 + 1];
            return;
        }
        throw std::length_error("Invalid byte amount for ip address");
    }

    ip_address::ip_address(const std::vector<unsigned char> &bytes, unsigned long scopeId) {
        if (bytes.size() != 16)
            throw std::length_error("Invalid bytes amount for ip address");
        m_family = inet6;
        for (int i = 0; i < 8; i++)
            m_values[i] = (bytes[i * 2] << 8) | bytes[i * 2 + 1];
        m_address = scopeId;
    }

    ip_address::ip_address(const std::vector<unsigned short> &values, unsigned long scopeId) {
        if (values.size() != 8)
            throw std::length_error("Invalid bytes amount for ip address");
        m_family = inet6;
        for (int i = 0; i < 8; i++)
            m_values[i] = values[i];
        m_address = scopeId;
    }

    ip_address::ip_address(unsigned int address) {
        m_address = address;
        m_family = inet;
    }

    unsigned short ip_address::host_to_network(unsigned short value) {
        return netsock::hostToNetwork(value);
    }

    unsigned int ip_address::host_to_network(unsigned int value) {
        return netsock::hostToNetwork(value);
    }

    unsigned long ip_address::host_to_network(unsigned long value) {
        return netsock::hostToNetwork(value);
    }

    unsigned long long ip_address::host_to_network(unsigned long long value) {
        return netsock::hostToNetwork(value);
    }

    unsigned short ip_address::network_to_host(unsigned short value) {
        return netsock::networkToHost(value);
    }

    unsigned int ip_address::network_to_host(unsigned int value) {
        return netsock::networkToHost(value);
    }

    unsigned long ip_address::network_to_host(unsigned long value) {
        return netsock::networkToHost(value);
    }

    unsigned long long ip_address::network_to_host(unsigned long long value) {
        return netsock::networkToHost(value);
    }

    ip_address ip_address::parse(const char *bytes, int size) {
        ip_address address;
        if (size == 4) {
            address.m_family = inet;
            address.m_address = *(unsigned int *)bytes;
            return address;
        }
        if (size == 16){
            address.m_family = inet6;
            for (size_t i = 0; i < 8; i++) {
                unsigned short value = 0;
                value |= (unsigned short)(bytes[i * 2] << 8);
                value |= (unsigned short)bytes[i * 2 + 1] & 0xFF;
                address.m_values[i] = value;
            }
            return address;
        }
        throw std::length_error("Invalid byte amount for ip address");
    }

    ip_address ip_address::parse(const std::string &address) {
        return ip_address(address);
    }

    void ip_address::to_bytes(char *bytes, int &size) const {
        if (family() == inet){
            if (size < 4)
                throw std::length_error("Byte pointer too small to store IPv4 address");
            writeBytesIPv4(bytes);
            size = 4;
        } else {
            if (size < 16)
                throw std::length_error("Byte pointer too small to store IPv6 address");
            writeBytesIPv6(bytes);
            size = 16;
        }
    }

    void ip_address::set_scope_id(unsigned int scopeId) {
        if (family() != inet6)
            return;
        m_address = scopeId;
    }

    ip_address ip_address::map_to_ipv6() const {
        if (family() != inet)
            throw invalid_operation("IP address family must be IPv4");
        std::vector<unsigned short> values(8);
        unsigned int address = network_to_host(m_address);
        values[5] = 0xFFFF;
        values[6] = address >> 16;
        values[7] = address;
        return {values, 0};
    }

    ip_address ip_address::map_to_ipv4() const {
        if (!ipv4_mapped_ipv6())
            throw invalid_operation("IP address must be an IPv4-Mapped IPv6");
        unsigned int address = 0;
        address |= m_values[6] << 16;
        address |= m_values[7];
        return ip_address(host_to_network(address));
    }

    unsigned int ip_address::address() const {
        if (family() != inet)
            return 0;
        return network_to_host(m_address);
    }

    address_family ip_address::family() const {
        return m_family;
    }

    unsigned int ip_address::scopeId() const {
        if (family() != inet6)
            return 0;
        return m_address;
    }

    void ip_address::writeBytesIPv6(char *out) const {
        size_t j = 0;
        for (unsigned short m_value : m_values){
            out[j++] = (char)((m_value >> 8) & 0xFF);
            out[j++] = (char)(m_value & 0xFF);
        }
    }

    void ip_address::writeBytesIPv4(char *out) const {
        *(unsigned int *)out = m_address;
    }

    bool ip_address::ipv4_mapped_ipv6() const {
        if (family() != inet6)
            return false;
        for (size_t i = 0; i < 5; i++)
            if (m_values[i] != 0)
                return false;
        return m_values[5] == 0xFFFF;
    }
}