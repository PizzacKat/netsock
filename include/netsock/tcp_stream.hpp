#ifndef netsock_tcp_stream
#define netsock_tcp_stream

#include "tcp_client.hpp"
#include <iostream>

namespace netsock {
    class tcp_streambuf : public std::streambuf {
    public:
        explicit tcp_streambuf(tcp_client &client);

        [[nodiscard]] const tcp_client &client() const;
        [[nodiscard]] tcp_client &client();
    private:
        int_type overflow(int_type __c) override;
        int_type underflow() override;

        std::streamsize xsputn(const char_type *__s, std::streamsize __n) override;
        std::streamsize xsgetn(char_type *__s, std::streamsize __n) override;

        tcp_client &_client;
    };

    class tcp_ostream : public std::ostream {
    public:
        explicit tcp_ostream(tcp_client &client);

        [[nodiscard]] bool connected() const;
        [[nodiscard]] const tcp_client &client() const;
        [[nodiscard]] tcp_client &client();
    private:
        tcp_streambuf _buf;
    };

    class tcp_istream : public std::istream {
    public:
        explicit tcp_istream(tcp_client &client);

        [[nodiscard]] bool connected() const;
        [[nodiscard]] const tcp_client &client() const;
        [[nodiscard]] tcp_client &client();
    private:
        tcp_streambuf _buf;
    };

    class tcp_stream : public std::iostream {
    public:
        explicit tcp_stream(tcp_client &client);

        [[nodiscard]] bool connected() const;
        [[nodiscard]] const tcp_client &client() const;
        [[nodiscard]] tcp_client &client();
    private:
        tcp_streambuf _buf;
    };
}

#endif //netsock_tcp_stream
