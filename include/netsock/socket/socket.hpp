#ifndef NETSOCK_SOCKET_HPP
#define NETSOCK_SOCKET_HPP

#if defined(__WIN32__) || defined(__WIN64__)
#include <wsipx.h>
#include <ws2tcpip.h>
#endif
#include "netsock/types.hpp"
#include "netsock/ip_address.hpp"
#include "netsock/errors.hpp"
#include "netsock/endpoint.hpp"
#include "netsock/select_list.hpp"

namespace netsock {
    class socket {
    public:
        socket();
        socket(address_family af, socket_type type, ip_protocol protocol);
        socket(socket_type type, ip_protocol protocol);
        explicit socket(socket_t socket, bool connected = true);
        socket(const socket &)= delete;
        socket &operator=(const socket &)= delete;
        socket(socket &&socket) noexcept;
        socket &operator=(socket &&socket) noexcept;

        [[nodiscard]] socket_t handle() const;
        void connect(const ip_address &address, unsigned short port);
        void connect(const endpoint &endpoint);
        void connect(const std::vector<ip_address> &addresses, unsigned short port);

        int write(const char *data, int length, size_t offset = 0);
        size_t write(const std::vector<char> &data);
        int write_to(const endpoint &endpoint, const char *data, int length, size_t offset = 0);
        size_t write_to(const endpoint &endpoint, const std::vector<char> &data);

        int read(char *out, int amount, size_t offset = 0);
        std::vector<char> read(size_t amount);
        int read_from(endpoint &endpoint, char *out, int amount, size_t offset = 0);
        std::vector<char> read_from(endpoint &endpoint, size_t amount);

        template <typename T>
        void set_option(int level, int option, const T &value){
            int res = setsockopt(m_socket, level, option, (const char *)&value, sizeof(T));
            if (res == -1){
                int err = LAST_ERROR();
                throw socket_exception(err, "setsockopt");
            }
        }

        template <typename T>
        void set_option(option::socket option, const T &value){
            set_option(SOL_SOCKET, option, value);
        }

        template <typename T>
        void set_option(option::tcp option, const T &value){
            set_option(IPPROTO_TCP, option, value);
        }

        template <typename T>
        void set_option(option::ipv6 option, const T &value){
            set_option(IPPROTO_IPV6, option, value);
        }

        template <typename T>
        T get_option(int level, int option){
            T value{};
            socklen_t len = sizeof(T);
            int res = getsockopt(m_socket, level, option, (char *)&value, &len);
            if (res == -1){
                int err = LAST_ERROR();
                throw socket_exception(err, "getsockopt");
            }
            return value;
        }

        template <typename T>
        T get_option(option::socket option){
            return get_option<T>(SOL_SOCKET, option);
        }

        template <typename T>
        T get_option(option::tcp option){
            return get_option<T>(IPPROTO_TCP, option);
        }

        template <typename T>
        T get_option(option::ipv6 option){
            return get_option<T>(IPPROTO_IPV6, option);
        }

        void bind(const endpoint &endpoint);

        void listen(int backlog = 3);

        socket accept();

        static void select(std::vector<const socket *> *read, std::vector<const socket *> *write, std::vector<const socket *> *error, int timeout);
        static void select(select_list *read, select_list *write, select_list *error, int timeout);

        void shutdown();
        void close();

        [[nodiscard]] bool valid() const;

        [[nodiscard]] netsock::address_family family() const;
        [[nodiscard]] socket_type type() const;
        [[nodiscard]] ip_protocol protocol() const;
        [[nodiscard]] bool connected() const;
        [[nodiscard]] bool listening() const;
        [[nodiscard]] endpoint local_endpoint() const;
        [[nodiscard]] endpoint remote_endpoint() const;
        [[nodiscard]] bool poll(ssize_t timeout, poll_mode mode);

        bool operator==(const socket &socket) const;

    private:
        [[nodiscard]] socket_address serialize(const endpoint &endpoint) const;
        void update_after_error(int code, bool disconnect = true);

        socket_t m_socket = netsock::invalid_socket;
        netsock::address_family m_family{};
        socket_type m_type{};
        ip_protocol m_protocol{};
        endpoint m_localEndpoint{};
        endpoint m_remoteEndpoint{};
        bool m_connected = false;
        bool m_listening = false;
    };
}

#endif //NETSOCK_SOCKET_HPP
