/*
Derivative from https://stackoverflow.com/a/16075550
*/

#ifndef _MESSAGE_QUEUE_HPP_
#define _MESSAGE_QUEUE_HPP_

#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <iostream>

template <class T> class MessageQueue
{
public:
    MessageQueue(void)
        : queue()
        , mutex()
        , condition_variable()
        , m_timeout(std::literals::chrono_literals::operator""ms(0)) {};

    MessageQueue(uint32_t timeout)
        : queue()
        , mutex()
        , condition_variable()
        , m_timeout(std::literals::chrono_literals::operator""ms(timeout)) {};

    ~MessageQueue(void)
    {
    }

    void push_back(T t)
    {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(t);
        condition_variable.notify_one();
    }

    bool pop_front(T* ret)
    {
        std::unique_lock<std::mutex> lock(mutex);

        if (queue.empty())
        {
            if (m_timeout == std::literals::chrono_literals::operator""s(0))
            {
                // unlock lock, then waits, and lastly locks again when signal is received
                condition_variable.wait(lock);
                std::cout << "here" << std::endl;
            }
            else
            {
                std::cv_status status = condition_variable.wait_for(lock, this->m_timeout);
                if (status == std::cv_status::timeout)
                {
                    return false;
                }
            }
        }

        *ret = queue.front();
        queue.pop();
        // Cant use lock_guard (why?), so need to ulock.
        lock.unlock();
        return true;
    }

    size_t size(void)
    {
        return queue.size();
    }

private:
    std::queue<T> queue;
    mutable std::mutex mutex;
    std::condition_variable condition_variable;
    std::chrono::duration<long double> m_timeout;
};

#endif // _MESSAGE_QUEUE_HPP_