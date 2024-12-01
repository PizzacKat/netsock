#include "netsock/tcp_stream.hpp"

#include "netsock/tcp_client.hpp"

namespace netsock {
    tcp_streambuf::tcp_streambuf(tcp_client &client): _client(client) {
        setg(_ibuf, _ibuf, _ibuf);
        setp(_obuf, _obuf + buffer_size);
    }

    const tcp_client &tcp_streambuf::client() const {
        return _client;
    }

    tcp_client &tcp_streambuf::client() {
        return _client;
    }

    std::streambuf::int_type tcp_streambuf::overflow(const int_type __c) {
        if (!_client.connected() || __c == traits_type::eof())
            return traits_type::eof();
        if (pptr() == eback()) {
            _client.send(std::as_bytes(std::span(_obuf)));
            setp(_obuf, _obuf + buffer_size);
        }
        const char_type c = traits_type::to_char_type(__c);
        *pptr() = c;
        return traits_type::to_int_type(c);
    }

    std::streambuf::int_type tcp_streambuf::underflow() {
        if (gptr() == egptr()) {
            if (!_client.connected())
                return traits_type::eof();
            const std::size_t received = _client.recv(std::as_writable_bytes(std::span(_ibuf, buffer_size)));
            setg(_ibuf, _ibuf, _ibuf + received);
        }
        const char_type c = *gptr();
        return traits_type::to_int_type(c);
    }

    std::streamsize tcp_streambuf::xsputn(const char_type *__s, std::streamsize __n) {
        if (!_client.connected())
            return traits_type::eof();
        if (__n > eback() - pptr()) {
            _client.send(std::as_bytes(std::span(_obuf, pptr() - pbase())));
            setp(_obuf, _obuf + buffer_size);
        }
        if (__n > buffer_size) {
            _client.send(std::as_bytes(std::span(__s, __n)));
            return __n;
        }
        traits_type::copy(pptr(), __s, __n);
        pbump(static_cast<int>(__n));
        return __n;
    }

    std::streamsize tcp_streambuf::xsgetn(char_type *__s, std::streamsize __n) {
        std::streamsize read = 0;
        read += std::min(__n, egptr() - gptr());
        traits_type::copy(__s, gptr(), read);
        gbump(static_cast<int>(read));
        while (__n > read) {
            if (!_client.connected())
                return traits_type::eof();
            read += static_cast<std::streamsize>(_client.recv(std::as_writable_bytes(std::span(__s + read, __n - read))));
        }
        return read;
    }

    std::streambuf::int_type tcp_streambuf::sync() {
        if (!client().connected())
            return traits_type::eof();
        std::size_t result = _client.send(std::as_bytes(std::span(pbase(), pptr() - pbase())));
        if (result == 0 && pptr() - pbase() != 0)
            return traits_type::eof();
        setp(_obuf, _obuf + buffer_size);
        return 0;
    }

    tcp_ostream::tcp_ostream(tcp_client &client): std::ostream(&_buf), _buf(client) {

    }

    bool tcp_ostream::connected() const {
        return _buf.client().connected();
    }

    const tcp_client & tcp_ostream::client() const {
        return _buf.client();
    }

    tcp_client & tcp_ostream::client() {
        return _buf.client();
    }

    tcp_istream::tcp_istream(tcp_client &client): std::istream(&_buf), _buf(client) {

    }

    bool tcp_istream::connected() const {
        return _buf.client().connected();
    }

    const tcp_client & tcp_istream::client() const {
        return _buf.client();
    }

    tcp_client & tcp_istream::client() {
        return _buf.client();
    }

    tcp_stream::tcp_stream(tcp_client &client): std::iostream(&_buf), _buf(client) {

    }

    bool tcp_stream::connected() const {
        return _buf.client().connected();
    }

    const tcp_client & tcp_stream::client() const {
        return _buf.client();
    }

    tcp_client & tcp_stream::client() {
        return _buf.client();
    }
}
