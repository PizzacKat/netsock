#include "netsock/tcp_stream.hpp"

#include "netsock/tcp_client.hpp"

namespace netsock {
    tcp_streambuf::tcp_streambuf(tcp_client &client): _client(client) {

    }

    const tcp_client & tcp_streambuf::client() const {
        return _client;
    }

    tcp_client & tcp_streambuf::client() {
        return _client;
    }

    std::streambuf::int_type tcp_streambuf::overflow(const int_type __c) {
        if (!_client.connected() || __c == traits_type::eof())
            return traits_type::eof();
        const char_type c = traits_type::to_char_type(__c);
        _client.send(std::as_bytes(std::span(&c, 1)));
        return traits_type::not_eof(__c);
    }

    std::streambuf::int_type tcp_streambuf::underflow() {
        if (!_client.connected())
            return traits_type::eof();
        char_type c;
        _client.recv(std::as_writable_bytes(std::span(&c, 1)));
        return traits_type::to_int_type(c);
    }

    std::streamsize tcp_streambuf::xsputn(const char_type *__s, const std::streamsize __n) {
        if (!_client.connected())
            return traits_type::eof();
        return static_cast<std::streamsize>(_client.send(std::as_bytes(std::span(__s, __n))));
    }

    std::streamsize tcp_streambuf::xsgetn(char_type *__s, const std::streamsize __n) {
        if (!_client.connected())
            return traits_type::eof();
        return static_cast<std::streamsize>(_client.recv(std::as_writable_bytes(std::span(__s, __n))));
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
