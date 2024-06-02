#include "netsock/endian.hpp"

namespace netsock::endian {
    char *smallToBig(char *to, const char *from, size_t size) {
        for (size_t i = 0; i < size; i++)
            to[i] = from[size - i - 1];
        return to;
    }

    char *bigToSmall(char *to, const char *from, size_t size) {
        for (size_t i = 0; i < size; i++)
            to[i] = from[size - i - 1];
        return to;
    }

    enum endianess endianess(){
        union {
            unsigned short value = 0xFF00;
            unsigned char bytes[2];
        } _a;
        return (enum endianess)(_a.bytes[0] == 0xFF);
    }
}