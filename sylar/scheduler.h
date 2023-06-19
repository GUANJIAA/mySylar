#ifndef __SYLAR_SCHEDULER_H__
#define __SYLAR_SCHEDULER_H__

#include "fiber.h"
#include "thread.h"
#include "mutex"

#include <memory>
#include <vector>
#include <functional>
#include <list>

namespace sylar
{
    class Scheduler
    {
    public:
        typedef std::shared_ptr<Scheduler> ptr;
        typedef Mutex MutexType;

        Scheduler(size_t threads = 1, bool use_caller = true, const std::string &name = "");
        virtual ~Scheduler();

        const std::string &getName() const { return m_name; }

        static Scheduler *GetThis();
        static Fiber *GetMainFiber();

        void start();
        void stop();

        template <class FiberOrCb>
        void schedule(FiberOrCb fc, int threadid = -1)
        {
            bool need_tickle = false;
            {
                MutexType::Lock lock(m_mutex);
                need_tickle = scheduleNoLock(fc, threadid)
            }
            if (need_tickle)
            {
                tickle();
            }
        }

        template <class Inputerator>
        void schedule(Inputerator begin, Inputerator end)
        {
            bool need_tickle = false;
            {
                MutexType::Lock lock(m_mutex);
                while (begin != end)
                {
                    tickle = scheduleNoLock(&*begin) || need_tickle;
                }
            }
            if (need_tickle)
            {
                tickle();
            }
        }

    protected:
        virtual void tickle();
        virtual bool stopping();
        virtual void idle();

        void run();

        void setThis();

    private:
        template <class FiberOrCb>
        bool scheduleNoLock(FiberOrCb fc, int threadid)
        {
            bool need_tickle = m_fibers.empty();
            FiberAndThread ft(fc, threadid);
            if (ft.fiber || ft.cb)
            {
                m_fibers.push_back(ft);
            }
            return need_tickle;
        }

    private:
        struct FiberAndThread
        {
            Fiber::ptr fiber;
            std::function<void()> cb;
            int threadId;

            FiberAndThread(Fiber::ptr f, int thr)
                : fiber(f), threadId(thr) {}

            FiberAndThread(Fiber::ptr *f, int thr)
                : threadId(thr)
            {
                fiber.swap(*f);
            }

            FiberAndThread(std::function<void()> f, int thr)
                : cb(f), threadId(thr) {}

            FiberAndThread(std::function<void()> *f, int thr)
                : threadId(thr)
            {
                cb.swap(*f);
            }

            FiberAndThread()
                : threadId(-1) {}

            void reset()
            {
                fiber = nullptr;
                cb = nullptr;
                threadId = -1;
            }
        };

    private:
        MutexType m_mutex;
        std::vector<Thread::ptr> m_threads;
        std::list<FiberAndThread> m_fibers;
        Fiber::ptr m_rootFiber;
        std::string m_name;

    protected:
        std::vector<int> m_threadIds;
        size_t m_threadCount = 0;
        size_t m_activeThreadCount = 0;
        size_t m_idleThreadCount = 0;
        bool m_stopping = true;
        bool m_autoStop = false;
        int m_rootThread = 0;
    };

};
#endif