#ifndef netsock_raw
#define netsock_raw

#include <iosfwd>
#include <istream>
#include <ostream>
#include <type_traits>

namespace netsock {
    template <typename T, typename = std::enable_if_t<std::is_standard_layout_v<T>>>
    struct raw_put {
        T value;
    };

    template <typename T, typename = std::enable_if_t<std::is_standard_layout_v<T>>>
    struct raw_get {
        T &value;
    };

    template <typename T, typename = std::enable_if_t<std::is_standard_layout_v<T>>>
    raw_put<T> put_raw(const T &value) {
        return {value};
    }

    template <typename T, typename = std::enable_if_t<std::is_standard_layout_v<T>>>
    raw_get<T> get_raw(T &value) {
        return {value};
    }

    template <typename T>
    std::ostream &operator<<(std::ostream &os, const raw_put<T> &put) {
        const auto data = reinterpret_cast<const char *>(&put.value);
        const std::streamsize size = sizeof(T);
        return os.write(data, size);
    }

    template <typename T>
    std::istream &operator>>(std::istream &is, const raw_get<T> &get) {
        const auto data = reinterpret_cast<char *>(&get.value);
        const std::streamsize size = sizeof(T);
        return is.read(data, size);
    }
}

#endif //netsock_raw
