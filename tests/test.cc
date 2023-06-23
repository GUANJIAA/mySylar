/*
 * @Author: GUANJIAA
 * @Date: 2023-06-03 11:50:52
 * @LastEditTime: 2023-06-03 16:19:02
 * @LastEditors: GUANJIAA
 * @Description:
 * @FilePath: /sylar/tests/test.cc
 * 版权声明
 */
#include <iostream>
#include "../sylar/log.h"
#include "../sylar/util.h"

int main(int argc, char **argv)
{
    sylar::Logger::ptr logger(new sylar::Logger);
    logger->addAppender(sylar::LogAppender::ptr(new sylar::StdoutLogAppender));
    // sylar::FileLogAppender::ptr file_appender(new sylar::FileLogAppender("./log.txt"));
    // sylar::LogFormatter::ptr fmt(new sylar::LogFormatter("%d%T%p%T%m%T%n"));
    // file_appender->setFormatter(fmt);
    // file_appender->setLevel(sylar::LogLevel::ERROR);

    // logger->addAppender(file_appender);

    // sylar::LogEvent::ptr event(new sylar::LogEvent(__FILE__, __LINE__, 0, sylar::GetThreadId(), sylar::GetFiberId(), time(0)));
    // event->getSS() << "hello sylar log";

    // logger->log(sylar::LogLevel::DEBUG, event);
    // std::cout << "hello sylar log" << std::endl;

    sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

    SYLAR_LOG_INFO(logger) << "test macro";
    SYLAR_LOG_ERROR(logger) << "test macro";
    SYLAR_LOG_DEBUG(logger) << "test macro";
    SYLAR_LOG_FATAL(logger) << "test macro";
    SYLAR_LOG_WARN(logger) << "test macro";

    SYLAR_LOG_FMT_DEBUG(logger, "test macro fmt error %s", "aa");
    SYLAR_LOG_FMT_ERROR(logger, "test macro fmt error %s", "aa");
    SYLAR_LOG_FMT_INFO(logger, "test macro fmt error %s", "aa");
    SYLAR_LOG_FMT_FATAL(logger, "test macro fmt error %s", "aa");
    SYLAR_LOG_FMT_WARN(logger, "test macro fmt error %s", "aa");

    auto l = sylar::LoggerMgr::GetInstance()->getLogger("xx");
    SYLAR_LOG_INFO(l) << "xxx";

    return 0;
}