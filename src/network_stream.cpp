#include "netsock/network_stream.hpp"

namespace netsock {
    network_streambuf::network_streambuf(socket &socket): m_socket(&socket), m_in(), m_out(){
        setp(m_out, m_out + buffSize - 1);
        setg(m_in, m_in, m_in);
    }

    network_streambuf::int_type network_streambuf::overflow(std::basic_streambuf<char>::int_type c) {
        if (!traits_type::eq_int_type(traits_type::eof(), c)){
            traits_type::assign(*pptr(), traits_type::to_char_type(c));
            pbump(1);
        }
        return sync() == 0 ? traits_type::not_eof(c) : traits_type::eof();
    }

    int network_streambuf::sync() {
        return pbase() == pptr() || m_socket->write(pbase(), pptr() - pbase()) == pptr() - pbase() ? 0 : -1;
    }

    network_streambuf::int_type network_streambuf::underflow() {
        int rc = m_socket->read(m_in, buffSize);
        if (rc == 0)
            return traits_type::eof();
        setg(m_in, m_in, m_in + rc);
        return traits_type::to_int_type(*gptr());
    }

    std::streamsize network_streambuf::xsgetn(char *s, std::streamsize n) {
        std::streamsize ret = 0;
        const std::streamsize buf_len = egptr() - gptr();
        if (buf_len){
            const std::streamsize remaining = n - ret;
            const std::streamsize len = std::min(remaining, buf_len);
            traits_type::copy(s, gptr(), len);
            ret += len;
            s += len;
            __safe_gbump(len);
        }
        while (ret < n) {
            const std::streamsize remaining = n - ret;
            int read = m_socket->read(s, (int)remaining);
            if (read == 0)
                break;
            ret += read;
            s += read;
        }
        return ret;
    }

    std::streamsize network_streambuf::xsputn(const char *s, std::streamsize n) {
        std::streamsize ret = 0;
        int res = sync();
        if (res == 0){
            m_socket->write(s, (int)n);
            ret = n;
        }
        setp(m_out, m_out + buffSize - 1);
        return ret;
    }

    network_stream::network_stream(netsock::socket &socket): std::iostream(&m_buf), m_buf(socket), m_socket(&socket){

    }

    const socket &network_stream::socket() const {
        return *m_socket;
    }

    socket &network_stream::socket() {
        return *m_socket;
    }
}