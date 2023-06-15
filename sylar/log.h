/***
 * @Author: GUANJIAA
 * @Date: 2023-06-03 13:57:49
 * @LastEditTime: 2023-06-03 14:16:51
 * @LastEditors: GUANJIAA
 * @Description: 日志系统
 * @FilePath: /sylar/sylar/log.h
 */

#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__

#include "util.h"
#include "thread.h"
#include "singleton.h"

#include <string>
#include <iostream>
#include <memory>
#include <map>
#include <list>
#include <vector>
#include <stdint.h>
#include <sstream>
#include <fstream>
#include <time.h>
#include <stdarg.h>
#include <string.h>

/***
 * @brief 使用流式方式将日志级别level写进logger里面
 * @param logger 日志输出器
 * @param level 日志级别
 * @return std::stringstream &
 */
#define SYLAR_LOG_LEVEL(logger, level)                                                                                       \
    if (logger->getLevel() <= level)                                                                                         \
    sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level, __FILE__, __LINE__, 0, sylar::GetThreadId(), \
                                                                 sylar::GetFiberId(), time(0),sylar::Thread::GetName())))                             \
        .getSS()

/***
 * @brief 使用流式方式将日志级别DEBUG写进logger里面
 * @return std::stringstream &
 */
#define SYLAR_LOG_DEBUG(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::DEBUG)

/***
 * @brief 使用流式方式将日志级别INFO写进logger里面
 * @return std::stringstream &
 */
#define SYLAR_LOG_INFO(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::INFO)

/***
 * @brief 使用流式方式将日志级别WARN写进logger里面
 * @return std::stringstream &
 */
#define SYLAR_LOG_WARN(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::WARN)

/***
 * @brief 使用流式方式将日志级别ERROR写进logger里面
 * @return std::stringstream &
 */
#define SYLAR_LOG_ERROR(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::ERROR)

/***
 * @brief 使用流式方式将日志级别FATAL写进logger里面
 * @return std::stringstream &
 */
#define SYLAR_LOG_FATAL(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::FATAL)

/***
 * @brief 使用格式化方式将日志级别level直接写入logger里面
 * @param logger 日志输出器
 * @param level 日志级别
 * @param fmt 日志格式
 * @return void
 */
#define SYLAR_LOG_FMT_LEVEL(logger, level, fmt, ...)                                                          \
    if (logger->getLevel() <= level)                                                                          \
    sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level,                               \
                                                                 __FILE__, __LINE__, 0, sylar::GetThreadId(), \
                                                                 sylar::GetFiberId(), time(0),sylar::Thread::GetName())))              \
        .getEvent()                                                                                           \
        ->format(fmt, __VA_ARGS__)

/***
 * @brief 使用格式化方式将日志级别DEBUG直接写入logger里面
 * @param logger 日志输出器
 * @param fmt 日志格式
 * @return void
 */
#define SYLAR_LOG_FMT_DEBUG(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::DEBUG, fmt, __VA_ARGS__)

/***
 * @brief 使用格式化方式将日志级别INFO直接写入logger里面
 * @param logger 日志输出器
 * @param fmt 日志格式
 * @return void
 */
#define SYLAR_LOG_FMT_INFO(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::INFO, fmt, __VA_ARGS__)

/***
 * @brief 使用格式化方式将日志级别WARN直接写入logger里面
 * @param logger 日志输出器
 * @param fmt 日志格式
 * @return void
 */
#define SYLAR_LOG_FMT_WARN(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::WARN, fmt, __VA_ARGS__)

/***
 * @brief 使用格式化方式将日志级别ERROR直接写入logger里面
 * @param logger 日志输出器
 * @param fmt 日志格式
 * @return void
 */
#define SYLAR_LOG_FMT_ERROR(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::ERROR, fmt, __VA_ARGS__)

/***
 * @brief 使用格式化方式将日志级别FATAL直接写入logger里面
 * @param logger 日志输出器
 * @param fmt 日志格式
 * @return void
 */
#define SYLAR_LOG_FMT_FATAL(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::FATAL, fmt, __VA_ARGS__)

#define SYLAR_LOG_ROOT() sylar::LoggerMgr::GetInstance()->getRoot()

#define SYLAR_LOG_NAME(name) sylar::LoggerMgr::GetInstance()->getLogger(name)

namespace sylar
{
    class LoggerManager;
    class Logger;

    // 日志级别类
    class LogLevel
    {
    public:
        // 日志级别枚举
        enum Level
        {
            UNKNOW = 0, // 未知级别
            DEBUG = 1,  // DEBUG级别
            INFO = 2,   // INFO级别
            WARN = 3,   // WARN级别
            ERROR = 4,  // ERROR级别
            FATAL = 5   // FATAL级别
        };

        /***
         * @brief 将枚举类型转换为string类型
         * @param level 日志级别
         */
        static const char *ToString(LogLevel::Level level);
        static LogLevel::Level FromString(const std::string &str);
    };

    // 日志事件类
    class LogEvent
    {
    public:
        // 定义LogEvent智能指针方便使用
        typedef std::shared_ptr<LogEvent> ptr;

        /***
         * @description: LogEvent的构造函数
         * @param logger 日志输出器
         * @param level 日志级别
         * @param file 文件名
         * @param line 行号
         * @param elapse 程序启动开始到现在的毫秒数
         * @param threadId 当前线程ID
         * @param fiber_id 当前协程ID
         * @param time 日志事件的时间
         */
        LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level, const char *file, int32_t line, uint32_t elapse, uint32_t threadid,
                 uint32_t fiber_id, uint64_t time, const std::string &thread_name);

        // 析构函数
        ~LogEvent();

        // 获取文件名
        const char *getFile() const { return m_file; }

        // 获取行号
        int32_t getLine() const { return m_line; }

        // 获取耗时
        uint32_t getElapse() const { return m_elapse; }

        // 获取当前线程id
        uint32_t getThread() const { return m_threadId; }

        // 获取当前协程id
        uint32_t getFiberId() const { return m_fiberId; }

        // 获取当前时间
        uint64_t getTime() const { return m_time; }

        const std::string &getThreadName() const { return m_threadName; }

        // 获取当前日志内容
        const std::string getContent() const { return m_ss.str(); }

        // 获取日志内容字符串流
        std::stringstream &getSS() { return m_ss; }

        // 获取日志输出器
        std::shared_ptr<Logger> getLogger() const { return m_logger; }

        // 获取日志级别
        LogLevel::Level getLevel() const { return m_level; }

        // 格式化写入日志内容
        void format(const char *fmt, ...);

        // 格式化写入日志内容
        void format(const char *fmt, va_list al);

    private:
        const char *m_file = nullptr;     // 文件名
        int32_t m_line = 0;               // 行号
        uint32_t m_elapse = 0;            // 程序启动开始到现在的毫秒数
        int32_t m_threadId = 0;           // 线程id
        uint32_t m_fiberId = 0;           // 协程id
        uint64_t m_time;                  // 时间戳
        std::stringstream m_ss;           // 日志内容流
        std::shared_ptr<Logger> m_logger; // 日志输出器
        LogLevel::Level m_level;          // 日志级别
        std::string m_threadName;
    };

    // 日志事件包装器
    class LogEventWrap
    {
    public:
        /***
         * @brief 构造函数
         * @param event 日志事件
         */
        LogEventWrap(LogEvent::ptr event);

        // 析构函数
        ~LogEventWrap();

        // 调用LogEvent的getSS函数获取LogEvent类的日志内容字符串流
        std::stringstream &getSS();

        // 获取日志事件
        LogEvent::ptr getEvent() { return m_event; }

    private:
        LogEvent::ptr m_event; // 日志事件
    };

    // 日志格式器
    class LogFormatter
    {
    public:
        // 定义LogFormatter类智能指针方便使用
        typedef std::shared_ptr<LogFormatter> ptr;

        /***
         * @brief 构造函数
         * @param pattern 格式模板
         * @details
         *  %m 消息
         *  %p 日志级别
         *  %r 累计毫秒数
         *  %c 日志名称
         *  %t 线程id
         *  %n 换行
         *  %d 时间
         *  %f 文件名
         *  %l 行号
         *  %T 制表符
         *  %F 协程id
         *  %N 线程名称
         *
         *  默认格式是 "%d%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
         */
        LogFormatter(const std::string &pattern);

        /***
         * @brief 返回格式化日志文本
         * @param logger 日志输出器
         * @param level 日志级别
         * @param event 日志事件
         */
        std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);

    public:
        // 日志格式化内容项
        class FormatItem
        {
        public:
            // 定义FormatItem类智能指针方便使用
            typedef std::shared_ptr<FormatItem> ptr;
            // 构造函数
            FormatItem(const std::string &fmt = "") {}
            // 虚析构函数
            virtual ~FormatItem() {}
            /***
             * @brief 定义纯虚函数，让子类继承，并且进行重写
             * @param logger 日志输出器
             * @param os 日志输出流
             * @param level 日志级别
             * @param event 日志事件
             */
            virtual void format(std::shared_ptr<Logger> logger, std::ostream &os, LogLevel::Level level, LogEvent::ptr event) = 0;
        };

        // 初始化，解析模板
        void init();

        bool isError() const { return m_error; }

        const std::string getPattern() const { return m_pattern; }

    private:
        std::string m_pattern;                // 格式模板
        std::vector<FormatItem::ptr> m_items; // 日志格式化内容项
        bool m_error = false;
    };

    // 日志输出目的地:基类
    class LogAppender
    {
        friend class Logger;

    public:
        // 定义LogAppender类智能指针方便使用
        typedef std::shared_ptr<LogAppender> ptr;
        typedef Mutex MutexType;

        virtual ~LogAppender() {}

        virtual std::string toYamlString() = 0;

        // 定义纯虚函数，让子类继承，并且进行重写
        virtual void Log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;

        // 设置日志格式
        void setFormatter(LogFormatter::ptr val);

        // 获取日志格式
        LogFormatter::ptr getFormatter();

        // 获取日志级别
        LogLevel::Level getLevel() const { return m_level; }

        // 设置日志级别
        void setLevel(LogLevel::Level level) { m_level = level; }

        void setFomatter(LogFormatter::ptr val);

    protected:
        LogLevel::Level m_level = LogLevel::DEBUG; // 日志级别，默认是DEBUG级别
        bool m_hasFormatter = false;
        MutexType m_mutex;
        LogFormatter::ptr m_formatter; // 日志格式器
    };

    // 日志输出器
    class Logger : public std::enable_shared_from_this<Logger>
    {
    public:
        friend class LoggerManager;
        typedef Mutex MutexType;
        // 声明Logger类的智能指针
        typedef std::shared_ptr<Logger> ptr;

        /***
         * @brief 构造函数
         * @param name 日志名称
         */
        Logger(const std::string &name = "root");

        /***
         * @brief 写入日志
         * @param level 日志级别
         * @param event 日志事件
         */
        void log(LogLevel::Level level, LogEvent::ptr event);

        /***
         * @brief 写入DEBUG级别日志
         * @param event 日志事件
         */
        void debug(LogEvent::ptr event);

        /***
         * @brief 写入INFO级别日志
         * @param event 日志事件
         */
        void info(LogEvent::ptr event);

        /***
         * @description: 写入WARN级别日志
         * @param event 日志事件
         */
        void warn(LogEvent::ptr event);

        /***
         * @brief 写入ERROR级别日志
         * @param event 日志事件
         */
        void error(LogEvent::ptr event);

        /***
         * @brief 写入FATAL级别日志
         * @param event 日志事件
         */
        void fatal(LogEvent::ptr event);

        /***
         * @brief 添加日志输出目的地
         * @param appender 日志输出目的地
         */
        void addAppender(LogAppender::ptr appender);

        /***
         * @brief 删除日志输出目的地
         * @param appender 日志输出目的地
         */
        void delAppender(LogAppender::ptr appender);

        void clearAppender();

        // 获取日志级别
        LogLevel::Level getLevel() const { return m_level; }

        // 设置日志级别
        void setLevel(LogLevel::Level val) { m_level = val; }

        // 获取日志名称
        const std::string getName() const { return m_name; }

        void setFormatter(LogFormatter::ptr val);
        void setFormatter(const std::string &val);
        LogFormatter::ptr getFormatter();

        std::string toYamlString();

    private:
        std::string m_name;                      // 日志名称
        LogLevel::Level m_level;                 // 日志级别
        std::list<LogAppender::ptr> m_appenders; // Appender集合
        LogFormatter::ptr m_formatter;           // 日志格式器
        MutexType m_mutex;
        Logger::ptr m_root;
    };

    // 输出到控制台的Appender的子类
    class StdoutLogAppender : public LogAppender
    {
    public:
        // 声明StdoutLogAppender类的智能指针
        typedef std::shared_ptr<StdoutLogAppender> ptr;

        std::string toYamlString() override;

        /***
         * @brief 将日志输出到控制台
         * @param logger 日志输出器
         * @param level 日志级别
         * @param event 日志事件
         */
        void Log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;

    private:
    };

    // 输出到文件的Appender的子类
    class FileLogAppender : public LogAppender
    {
    public:
        // 声明StdoutLogAppender类的智能指针
        typedef std::shared_ptr<FileLogAppender> ptr;

        std::string toYamlString() override;

        /***
         * @brief 构造函数，初始化文件名
         * @param filename 文件名
         */
        FileLogAppender(const std::string &filename);

        /***
         * @brief 将日志输出到文件
         * @param logger 日志输出器
         * @param level 日志级别
         * @param event 日志事件
         */
        void Log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;

        // 重新打开文件，文件打开成功返回true
        bool reopen();

    private:
        std::string m_filename;     // 文件名
        std::ofstream m_filestream; // 文件流
        uint64_t m_lastTime = 0;
    };

    // 日志器管理类
    class LoggerManager
    {
    public:
        typedef Mutex MutexType;
        // 构造函数
        LoggerManager();

        /***
         * @brief 获取日志输出器
         * @param name 日志输出器名称
         */
        Logger::ptr getLogger(const std::string &name);

        void init();
        Logger::ptr getRoot() const { return m_root; }

        std::string toYamlString();

    private:
        MutexType m_mutex;
        std::map<std::string, Logger::ptr> m_loggers; // 日志输出器map
        Logger::ptr m_root;                           // 主日志输出器
    };

    // 日志器管理类单例模式
    typedef sylar::Singleton<LoggerManager> LoggerMgr;

} // namespace sylar

#endif