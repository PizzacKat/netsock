#ifndef NETSOCK_ENDPOINT_HPP
#define NETSOCK_ENDPOINT_HPP

#include <memory>
#include "socket_address.hpp"
#include "errors.hpp"

namespace netsock {
    class endpoint_storage {
    public:
        virtual void *from_address(const socket_address &address)= 0;
        [[nodiscard]] virtual std::shared_ptr<endpoint_storage> copy() const= 0;
        [[nodiscard]] virtual const std::type_info &type() const= 0;
        [[nodiscard]] virtual address_family family() const= 0;
        [[nodiscard]] virtual socket_address serialize() const= 0;
        virtual ~endpoint_storage() {

        }
    };

    template <typename Endpoint>
    class endpoint_typeeraser : public endpoint_storage {
    public:
        explicit endpoint_typeeraser(const Endpoint &endpoint){
            m_endpoint = endpoint;
        }

        void *from_address(const netsock::socket_address &address) override{
            m_endpoint = Endpoint(address);
            return &m_endpoint;
        }

        [[nodiscard]] std::shared_ptr<endpoint_storage> copy() const override{
            return std::dynamic_pointer_cast<endpoint_storage>(std::make_shared<endpoint_typeeraser>(m_endpoint));
        }

        [[nodiscard]] const std::type_info &type() const override{
            return typeid(Endpoint);
        }

        [[nodiscard]] address_family family() const override{
            return m_endpoint.family();
        }

        [[nodiscard]] socket_address serialize() const override{
            return m_endpoint.serialize();
        }

        Endpoint &get_endpoint(){
            return m_endpoint;
        }

        [[nodiscard]] const Endpoint &get_endpoint() const{
            return m_endpoint;
        }
    private:
        Endpoint m_endpoint;
    };

    class endpoint {
    public:
        endpoint()= default;
        template <typename Endpoint>
        endpoint(const Endpoint &endpoint){
            m_storage = std::dynamic_pointer_cast<endpoint_storage>(std::make_shared<endpoint_typeeraser<Endpoint>>(endpoint));
        }
        endpoint(const endpoint &endpoint);

        endpoint &from_address(const netsock::socket_address &address);

        [[nodiscard]] const std::type_info &type() const;

        [[nodiscard]] address_family family() const;

        [[nodiscard]] socket_address serialize() const;

        template <typename Endpoint>
        Endpoint &cast(){
            if (!m_storage)
                throw invalid_operation("Endpoint is null");
            return std::dynamic_pointer_cast<endpoint_typeeraser<Endpoint>>(m_storage)->get_endpoint();
        }

        template <typename Endpoint>
        [[nodiscard]] const Endpoint &cast() const{
            if (!m_storage)
                throw invalid_operation("Endpoint is null");
            if (type() != typeid(Endpoint))
                throw std::bad_cast();
            return std::dynamic_pointer_cast<endpoint_typeeraser<Endpoint>>(m_storage)->get_endpoint();
        }

        bool operator==(const std::nullptr_t &) const;

        template <typename Endpoint>
        bool operator==(const Endpoint &endpoint) const{
            return cast<Endpoint>() == endpoint;
        }
    private:
        std::shared_ptr<endpoint_storage> m_storage{nullptr};
    };
}

#endif //NETSOCK_ENDPOINT_HPP
