#ifndef ENDIAN_HPP
#define ENDIAN_HPP
#include <cstdint>
#include <cstring>

namespace netsock {
    enum endianness {
        big_endian, small_endian
    };

    struct current_endianness {
        static constexpr uint16_t _value = 0x2040;
        static constexpr endianness value = (_value & 0xFF) == 0x20 ? big_endian : small_endian;
    };

    template <endianness, endianness>
    struct endian_converter {
        void *copy(void *dst, const void *src, const std::size_t n) const {
            auto *d = (uint8_t *)dst + n;
            const auto *s = (const uint8_t *)src;
            for (std::size_t i = 0; i < n; i++)
                *--d = *s++;
            return dst;
        }

        [[nodiscard]] uint16_t operator()(const uint16_t value) const {
            return value >> 8 & 0xFF | value << 8 & 0xFF00;
        }

        [[nodiscard]] uint32_t operator()(const uint32_t value) const {
            return value >> 24 & 0xFF | value >> 8 & 0xFF00 | value << 8 & 0xFF0000 | value << 24 & 0xFF000000;
        }

        [[nodiscard]] uint64_t operator()(const uint64_t value) const {
            return value >> 56 & 0xFF | value >> 40 & 0xFF00 | value >> 24 & 0xFF0000 | value >> 8 & 0xFF000000 | value << 8 & 0xFF00000000 | value << 24 & 0xFF0000000000 | value << 40 & 0xFF000000000000 | value << 56 & 0xFF00000000000000;
        }
    };

    template <endianness Same>
    struct endian_converter<Same, Same> {
        void *copy(void *dst, const void *src, const std::size_t n) const {
            return memcpy(dst, src, n);
        }

        [[nodiscard]] uint16_t operator()(const uint16_t value) const {
            return value;
        }

        [[nodiscard]] uint32_t operator()(const uint32_t value) const {
            return value;
        }

        [[nodiscard]] uint64_t operator()(const uint64_t value) const {
            return value;
        }
    };
    static constexpr endianness val = current_endianness::value;
    static constexpr endian_converter<current_endianness::value, big_endian> host_to_network;
    static constexpr endian_converter<big_endian, current_endianness::value> network_to_host;
}

#endif //ENDIAN_HPP
