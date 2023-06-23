#include "../sylar/sylar.h"

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

void test_fiber()
{
    static int s_count = 5;
    SYLAR_LOG_INFO(g_logger) << "in test_fiber s_count=" << s_count;

    sleep(1);

    if (--s_count >= 0)
    {
        sylar::Scheduler::GetThis()->schedule(test_fiber, sylar::GetThreadId());
    }
}

int main(int argc, char **argv)
{
    SYLAR_LOG_INFO(g_logger) << "main";
    sylar::Scheduler::ptr sc(new sylar::Scheduler(3, true, "test"));
    // SYLAR_LOG_INFO(g_logger) << "schedule";
    // sc->schedule(test_fiber);
    sc->start();
    sleep(2);
    SYLAR_LOG_INFO(g_logger) << "schedule";
    sc->schedule(test_fiber);
    // SYLAR_LOG_INFO(g_logger) << "stop";
    sc->stop();
    SYLAR_LOG_INFO(g_logger) << "over";
    return 0;
}