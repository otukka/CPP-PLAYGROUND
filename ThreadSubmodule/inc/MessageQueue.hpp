/*
Derivative from https://stackoverflow.com/a/16075550
*/

#ifndef MESSAGE_QUEUE_HPP_
#define MESSAGE_QUEUE_HPP_

#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <iostream>

namespace
{
using namespace std::chrono_literals;
using millis = std::chrono::duration<uint32_t, std::milli>;
}

template<class T>
class MessageQueue
{
private:
    std::queue<T> queue;
    mutable std::mutex mutex;
    std::condition_variable condition_variable;
    millis m_timeout;

public:
    MessageQueue(uint32_t timeout) : queue(), mutex(), condition_variable(), m_timeout(timeout) {}

    ~MessageQueue(void) {}

    void push_back(T t)
    {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(t);
        condition_variable.notify_one();
    }

    bool pop_front(T& ret)
    {
        std::unique_lock<std::mutex> lock(mutex);

        if (queue.empty())
        {

            std::cv_status status = condition_variable.wait_for(lock, this->m_timeout);
            if (status == std::cv_status::timeout)
            {
                return false;
            }
        }

        ret = queue.front();
        queue.pop();
        lock.unlock();
        return true;
    }

    size_t size()
    {
        return queue.size();
    }
};

#endif  // MESSAGE_QUEUE_HPP_