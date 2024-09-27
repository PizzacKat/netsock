#ifndef NSTREAM_HPP
#define NSTREAM_HPP

#include <algorithm>
#include <array>
#include <iostream>
#include "socket.hpp"

namespace netsock {
    class networkbuf : public std::basic_streambuf<std::byte>{
    public:
        explicit networkbuf(netsock::socket &socket): _rdbuf(), _wrbuf(), _socket(socket) {
            setg(_rdbuf, _rdbuf, _rdbuf);
            setp(_wrbuf, _wrbuf + buffer_size);
        }
    protected:
        int_type overflow(const int_type c) override {
            try {
                if (const std::size_t len = pptr() - pbase()) {
                    if (_socket.send(pbase(), len) != len)
                        return traits_type::eof();
                }
                setp(_wrbuf, _wrbuf + buffer_size);
                if (!traits_type::eq_int_type(traits_type::eof(), c))
                    sputc(traits_type::to_char_type(c));
                return traits_type::not_eof(c);
            } catch (const socket_error &) {
                return traits_type::eof();
            }
        }

        int_type underflow() override {
            try {
                const std::size_t received = _socket.recv(_rdbuf, buffer_size);
                if (!received)
                    return traits_type::eof();
                setg(_rdbuf, _rdbuf, _rdbuf + received);
                return traits_type::to_int_type(*gptr());
            } catch (const socket_error &) {
                return traits_type::eof();
            }
        }

        std::streamsize xsputn(const char_type *s, const std::streamsize n) override {
            try {
                if (epptr() - pptr() <= n) {
                    traits_type::copy(pptr(), s, n);
                    pbump((int)n);
                    return n;
                }
                if (const std::streamsize len = pptr() - pbase()) {
                    if (_socket.send(pbase(), len) != len)
                        return traits_type::eof();
                    setp(_wrbuf, _wrbuf + buffer_size);
                }
                return (std::streamsize)_socket.send(s, n);
            } catch (const socket_error &) {
                return traits_type::eof();
            }
        }

        std::streamsize xsgetn(char_type *s, std::streamsize n) override {
            try {
                std::streamsize read = 0;
                if (const std::streamsize len = std::min(egptr() - gptr(), n)) {
                    traits_type::move(s, gptr(), len);
                    gbump((int)len);
                    n -= len;
                    s += len;
                    read += len;
                }
                if (n == 0)
                    return read;
                return read + (std::streamsize)_socket.recv(s, n);
            } catch (const socket_error &) {
                return traits_type::eof();
            }
        }

        int sync() override {
            return pptr() == pbase() || _socket.send(pptr(), pptr() - pbase()) ? 0 : -1;
        }
    private:
        static constexpr std::size_t buffer_size = 1024;

        std::byte _rdbuf[buffer_size];
        std::byte _wrbuf[buffer_size];
        netsock::socket &_socket;
    };

    class nstream : public std::basic_iostream<std::byte> {
    public:
        explicit nstream(netsock::socket &socket): basic_iostream(&_buf), _buf(socket), _socket(socket) {

        }

        [[nodiscard]] netsock::socket &socket() const {
            return _socket;
        }
    private:
        networkbuf _buf;
        netsock::socket &_socket;
    };
}

#endif //NSTREAM_HPP
