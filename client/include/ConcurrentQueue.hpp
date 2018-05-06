#ifndef CONCURRENTQUEUE_H
#define CONCURRENTQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template<class T>
class ConcurrentQueue
{
    public:
        ConcurrentQueue(){}
        virtual ~ConcurrentQueue(){}

        void push(T item)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_queue.push(item);
            m_condpop.notify_one();
        }

        bool try_pop(T& out)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            if(m_queue.empty())
                return false;
            out = std::move(m_queue.front());
            m_queue.pop();
            return true;
        }

        void pop(T& out)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            while(m_queue.empty())
            {
                m_condpop.wait(lock);
            }
            out = std::move(m_queue.front());
            m_queue.pop();

        }

        size_t size()
        {
          std::unique_lock<std::mutex> lock(m_mutex);
          return m_queue.size();
        }
    protected:

    private:
        std::queue<T> m_queue;
        std::mutex m_mutex;
        std::condition_variable m_condpop;
};

#endif // CONCURRENTQUEUE_H
