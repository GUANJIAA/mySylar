#include "../sylar/sylar.h"
#include "../sylar/iomanager.h"

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

void test_fiber()
{
    SYLAR_LOG_INFO(g_logger) << "test_fiber";

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(sock, F_SETFL, O_NONBLOCK);

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    inet_pton(AF_INET, "14.119.104.189", &addr.sin_addr.s_addr);

    connect(sock, (const sockaddr *)&addr, sizeof(addr));
    sylar::IOManager::GetThis()->addEvent(sock, sylar::IOManager::READ, []()
                                          { SYLAR_LOG_INFO(g_logger) << "read callback"; });
    sylar::IOManager::GetThis()->addEvent(sock, sylar::IOManager::WRITE, [&]()
                                          { SYLAR_LOG_INFO(g_logger) << "write callback";
                                          //close(sock);
                                          sylar::IOManager::GetThis()->cancelEvent(sock,sylar::IOManager::READ);
                                          close(sock); });
}

void test1()
{
    sylar::IOManager iom(2, false);
    iom.schedule(test_fiber);
}

sylar::Timer::ptr s_timer;
void test_timer()
{
    sylar::IOManager iom(2);
    s_timer = iom.addTimer(
        1000, []()
        {
        static int i = 0;
        SYLAR_LOG_INFO(g_logger) << "hello timer i=" << i;
        if (++i == 3)
        {
            s_timer->reset((uint64_t)2000,true);
        } },
        true);
}

int main(int argc, char **argv)
{
    // test1();
    test_timer();
    return 0;
}