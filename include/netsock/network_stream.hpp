#ifndef NETSOCK_NETWORK_STREAM_HPP
#define NETSOCK_NETWORK_STREAM_HPP

#include <iostream>
#include "netsock/socket/socket.hpp"

namespace netsock {
    class network_streambuf : public std::streambuf {
    public:
        explicit network_streambuf(socket &socket);
    protected:
        int_type overflow(std::basic_streambuf<char>::int_type c = traits_type::eof()) override;
        int sync() override;
        int_type underflow() override;
        std::streamsize xsgetn(char *s, std::streamsize n) override;
        std::streamsize xsputn(const char *s, std::streamsize n) override;
    private:
        static constexpr size_t buffSize = 2048;
        socket *m_socket;
        char m_in[buffSize];
        char m_out[buffSize];
    };

class network_stream : public std::iostream {
    public:
        explicit network_stream(netsock::socket &socket);
        [[nodiscard]] const netsock::socket &socket() const;
        netsock::socket &socket();
    private:
        network_streambuf m_buf;
        netsock::socket *m_socket = nullptr;
    };
}

#endif //NETSOCK_NETWORK_STREAM_HPP
