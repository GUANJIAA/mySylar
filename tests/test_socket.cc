#include "../sylar/sylar.h"
#include "../sylar/socket.h"

static sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

void test_socket()
{
    // std::vector<sylar::Address::ptr> addrs;
    // sylar::Address::Lookup(addrs, "www.baidu.com", AF_INET);
    // sylar::IPAddress::ptr addr;
    // for(auto& i : addrs) {
    //     SYLAR_LOG_INFO(g_looger) << i->toString();
    //     addr = std::dynamic_pointer_cast<sylar::IPAddress>(i);
    //     if(addr) {
    //         break;
    //     }
    // }
    sylar::IPAddress::ptr addr = sylar::Address::LookupIPAdress("www.baidu.com");
    if (addr)
    {
        SYLAR_LOG_INFO(g_logger) << "get address: " << addr->toString();
    }
    else
    {
        SYLAR_LOG_ERROR(g_logger) << "get address fail";
        return;
    }

    sylar::Socket::ptr sock = sylar::Socket::CreateTCP(addr);
    addr->setPort(80);
    SYLAR_LOG_INFO(g_logger) << "addr=" << addr->toString();
    if (!sock->connect(addr))
    {
        SYLAR_LOG_ERROR(g_logger) << "connect " << addr->toString() << " fail";
        return;
    }
    else
    {
        SYLAR_LOG_INFO(g_logger) << "connect " << addr->toString() << " connected";
    }

    const char buff[] = "GET / HTTP/1.0\r\n\r\n";
    int rt = sock->send(buff, sizeof(buff));
    if (rt <= 0)
    {
        SYLAR_LOG_INFO(g_logger) << "send fail rt=" << rt;
        return;
    }

    std::string buffs;
    buffs.resize(4096);
    rt = sock->recv(&buffs[0], buffs.size());

    if (rt <= 0)
    {
        SYLAR_LOG_INFO(g_logger) << "recv fail rt=" << rt;
        return;
    }

    buffs.resize(rt);
    SYLAR_LOG_INFO(g_logger) << buffs;
}

void test2()
{
    sylar::IPAddress::ptr addr = sylar::Address::LookupIPAdress("www.baidu.com:80");
    if (addr)
    {
        SYLAR_LOG_INFO(g_logger) << "get address: " << addr->toString();
    }
    else
    {
        SYLAR_LOG_ERROR(g_logger) << "get address fail";
        return;
    }

    sylar::Socket::ptr sock = sylar::Socket::CreateTCP(addr);
    if (!sock->connect(addr))
    {
        SYLAR_LOG_ERROR(g_logger) << "connect " << addr->toString() << " fail";
        return;
    }
    else
    {
        SYLAR_LOG_INFO(g_logger) << "connect " << addr->toString() << " connected";
    }

    uint64_t ts = sylar::GetCurrentUS();
    for (size_t i = 0; i < 10000000000ul; ++i)
    {
        if (int err = sock->getError())
        {
            SYLAR_LOG_INFO(g_logger) << "err=" << err << " errstr=" << strerror(err);
            break;
        }

        // struct tcp_info tcp_info;
        // if(!sock->getOption(IPPROTO_TCP, TCP_INFO, tcp_info)) {
        //     SYLAR_LOG_INFO(g_logger) << "err";
        //     break;
        // }
        // if(tcp_info.tcpi_state != TCP_ESTABLISHED) {
        //     SYLAR_LOG_INFO(g_logger)
        //             << " state=" << (int)tcp_info.tcpi_state;
        //     break;
        // }
        static int batch = 10000000;
        if (i && (i % batch) == 0)
        {
            uint64_t ts2 = sylar::GetCurrentUS();
            SYLAR_LOG_INFO(g_logger) << "i=" << i << " used: " << ((ts2 - ts) * 1.0 / batch) << " us";
            ts = ts2;
        }
    }
}

int main(int argc, char **argv)
{
    sylar::IOManager iom;
    iom.schedule(test_socket);
    // iom.schedule(test2);

    return 0;
}