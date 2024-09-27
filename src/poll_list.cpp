#include "netsock/poll_list.hpp"

#include <algorithm>
#include <netsock/impl.hpp>

namespace netsock {
    poll_list::poll_list()= default;
    poll_list::poll_list(const std::initializer_list<poll_entry> entries): _entries(entries.begin(), entries.end()) {

    }

    void poll_list::push(const poll_entry &entry) {
        _entries.push_back(entry);
    }

    void poll_list::push(socket &socket, int events) {
        _entries.emplace_back(socket, events);
    }

    void poll_list::remove(const socket &socket) {
        const auto it = std::find_if(_entries.begin(), _entries.end(), [&socket](const poll_entry &e) {
            return *e.socket == socket;
        });
        if (it != _entries.end())
            _entries.erase(it);
    }

    std::size_t poll_list::poll(const std::chrono::milliseconds timeout) {
        return impl::poll(*this, timeout);
    }

    std::size_t poll_list::size() const {
        return _entries.size();
    }

    poll_entry &poll_list::operator[](const std::size_t index) {
        return _entries[index];
    }

    int poll_list::result(const socket &socket) const {
        const auto it = std::find_if(_entries.begin(), _entries.end(), [&socket](const poll_entry &e) {
            return *e.socket == socket;
        });
        if (it != _entries.end())
            return it->result;
        return 0;
    }

}
