#ifndef CONCURRENTQUEUE_H
#define CONCURRENTQUEUE_H

#include <queue>
#include <mutex>

template<class T>
class ConcurrentQueue
{
    public:
        ConcurrentQueue();
        virtual ~ConcurrentQueue();

    protected:

    private:
        std::queue<T> m_queue;
        std::mutex m_mutex;
};

#endif // CONCURRENTQUEUE_H
