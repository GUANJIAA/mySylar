#pragma once
#ifndef __SYLAR_UTIL_H__
#define __SYLAR_UTIL_H__

#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <vector>
#include <string>

#include <unistd.h>
#include <stdint.h>

namespace sylar
{
    pid_t GetThreadId();
    uint32_t GetFiberId();

    void Backtrace(std::vector<std::string> &bt, int size, int skip = 1);
    std::string BacktraceToString(int size = 64, int skip = 2, const std::string &prefix = "");

    // 时间ms
    uint64_t GetCurrentMS();
    uint64_t GetCurrentUS();

    std::string timeToStr(time_t ts = time(0), const std::string &format = "%Y-%m-%d %H:%M:%S");

    class FSUtil
    {
    public:
        static void ListAllFile(std::vector<std::string> &files,
                                const std::string &path,
                                const std::string &subfix);

        static bool Mkdir(const std::string &dirname);

        static bool IsRunningPidfile(const std::string &pidfile);
    };

};

#endif