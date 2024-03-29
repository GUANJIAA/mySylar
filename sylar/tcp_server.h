#ifndef __SYLAR_TCP_SERVER_H__
#define __SYLAR_TCP_SERVER_H__

#include <memory>
#include <vector>
#include <string>
#include <stdint.h>
#include <functional>

#include "iomanager.h"
#include "socket.h"
#include "address.h"
#include "noncopyable.h"

namespace sylar
{
    class TcpServer : public std::enable_shared_from_this<TcpServer>, Noncopyable
    {
    public:
        typedef std::shared_ptr<TcpServer> ptr;

        TcpServer(sylar::IOManager *worker = sylar::IOManager::GetThis(),
                  sylar::IOManager *io_worker = sylar::IOManager::GetThis(),
                  sylar::IOManager *accept_worker = sylar::IOManager::GetThis());
        virtual ~TcpServer();

        virtual bool bind(sylar::Address::ptr addr);
        virtual bool bind(const std::vector<Address::ptr> &addrs,
                          std::vector<Address::ptr> &fails);
        virtual bool start();
        virtual bool stop();

        uint64_t getReadTimeout() const { return m_readTimeout; }
        std::string getName() const { return m_name; }
        void setReadTimeout(uint64_t val) { m_readTimeout = val; }
        void setName(const std::string &val) { m_name = val; }

        bool isStop() const { return m_isStop; }

    protected:
        virtual void handleClient(Socket::ptr client);
        virtual void startAccept(Socket::ptr sock);

        std::string m_type = "tcp";

    private:
        std::vector<Socket::ptr> m_socks;
        IOManager *m_worker;
        IOManager *m_ioWorker;
        IOManager *m_acceptWorker;
        uint64_t m_readTimeout;
        std::string m_name;
        bool m_isStop;
    };
}

#endif