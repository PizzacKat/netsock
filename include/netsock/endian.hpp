#ifndef NETSOCK_ENDIAN_HPP
#define NETSOCK_ENDIAN_HPP

#include <cstddef>

namespace netsock::endian {
    enum endianess {
        small, big
    };

    char *smallToBig(char *to, const char *from, size_t size);
    char *bigToSmall(char *to, const char *from, size_t size);
    endianess endianess();
}

#endif //NETSOCK_ENDIAN_HPP
