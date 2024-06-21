#ifndef NETSOCK_ERRORS_HPP
#define NETSOCK_ERRORS_HPP

#include "../../src/impl/impl.hpp"

#include <stdexcept>
#if defined(__WIN32__) || defined(__WIN64__)
#define ERRNAME(ERR) WSA##ERR
#define LAST_ERROR() WSAGetLastError()
#elif defined(__unix__)
#define ERRNAME(ERR) ERR
#define LAST_ERROR() errno
#endif

namespace netsock {
    using std::runtime_error;

    enum socket_error {
        INTR = ERRNAME(EINTR),
        BADF = ERRNAME(EBADF),
        ACCESS = ERRNAME(EACCES),
        FAULT = ERRNAME(EFAULT),
        INVAL = ERRNAME(EINVAL),
        MFILE = ERRNAME(EMFILE),
        WOULDBLOCK = ERRNAME(EWOULDBLOCK),
        INPROGRESS = ERRNAME(EINPROGRESS),
        ALREADY = ERRNAME(EALREADY),
        NOTSOCK = ERRNAME(ENOTSOCK),
        DESTADDRREQ = ERRNAME(EDESTADDRREQ),
        MSGSIZE = ERRNAME(EMSGSIZE),
        PROTOTYPE = ERRNAME(EPROTOTYPE),
        NOPROTOOPT = ERRNAME(ENOPROTOOPT),
        PROTONOSUPPORT = ERRNAME(EPROTONOSUPPORT),
        SOCKTNOSUPPORT = ERRNAME(ESOCKTNOSUPPORT),
        OPNOTSUPP = ERRNAME(EOPNOTSUPP),
        PFNOSUPPORT = ERRNAME(EPFNOSUPPORT),
        AFNOSUPPORT = ERRNAME(EAFNOSUPPORT),
        ADDRINUSE = ERRNAME(EADDRINUSE),
        ADDRNOTAVAIL = ERRNAME(EADDRNOTAVAIL),
        NETDOWN = ERRNAME(ENETDOWN),
        NETUNREACH = ERRNAME(ENETUNREACH),
        NETRESET = ERRNAME(ENETRESET),
        CONNABORTED = ERRNAME(ECONNABORTED),
        CONNRESET = ERRNAME(ECONNRESET),
        NOBUFS = ERRNAME(ENOBUFS),
        ISCONN = ERRNAME(EISCONN),
        NOTCONN = ERRNAME(ENOTCONN),
        SHUTDOWN = ERRNAME(ESHUTDOWN),
        TOOMANYREFS = ERRNAME(ETOOMANYREFS),
        TIMEDOUT = ERRNAME(ETIMEDOUT),
        CONNREFUSED = ERRNAME(ECONNREFUSED),
        LOOP = ERRNAME(ELOOP),
        NAMETOOLONG = ERRNAME(ENAMETOOLONG),
        HOSTDOWN = ERRNAME(EHOSTDOWN),
        HOSTUNREACH = ERRNAME(EHOSTUNREACH),
        NOTEMPTY = ERRNAME(ENOTEMPTY),
        USERS = ERRNAME(EUSERS),
        DQUOT = ERRNAME(EDQUOT),
        STALE = ERRNAME(ESTALE),
        REMOTE = ERRNAME(EREMOTE),
#if defined(__WIN32__) || defined(__WIN64__)
        CANCELED = ERRNAME(ECANCELLED),
#elif defined(__unix__)
        CANCELED = ERRNAME(ECANCELED),
#endif
    };

    class socket_exception : public std::exception {
    public:
        explicit socket_exception(impl::result_t code): socket_exception(code, {}){

        }

        socket_exception(impl::result_t code, const std::string &message): code(code){
            if (message.empty())
                m_what = std::to_string(code);
            else
                m_what = message + " (" + std::to_string(code) + ")";
        }

        [[nodiscard]] const char *what() const noexcept override{
            return m_what.c_str();
        }

        impl::result_t code;
    private:
        std::string m_what;
    };

    class invalid_operation : public runtime_error {
    public:
        using runtime_error::runtime_error;
    };
}


#endif //NETSOCK_ERRORS_HPP
