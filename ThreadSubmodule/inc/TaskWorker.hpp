
#ifndef _TASK_WORKER_HPP_
#define _TASK_WORKER_HPP_

#include <unordered_map>
#include <memory>
#include <thread>
#include <csignal>
#include "MessageQueue.hpp"

class TaskWorker
{
protected:
    std::thread m_thread;
    std::sig_atomic_t m_running;
    std::unordered_map<std::string, std::shared_ptr<MessageQueue<std::string>>> m_queues;

public:
    // TaskWorker();
    virtual ~TaskWorker() = default;
    virtual bool init() = 0;
    virtual bool run() = 0;
    virtual bool stop() = 0;

    void createQueue(std::string queue, uint32_t timeout)
    {
        m_queues.insert(std::pair<std::string, std::shared_ptr<MessageQueue<std::string>>>(
            queue, std::shared_ptr<MessageQueue<std::string>>(new MessageQueue<std::string>(timeout))));
    }

    std::shared_ptr<MessageQueue<std::string>> getQueue(std::string queue)
    {
        auto it = m_queues.find(queue);
        return it->second;
    }

    void setQueue(std::string queue, std::shared_ptr<MessageQueue<std::string>> shr_ptr)
    {
        m_queues.insert(std::pair<std::string, std::shared_ptr<MessageQueue<std::string>>>(queue, shr_ptr));
    }

    void pushMessage(std::string queue, std::string msg)
    {
        auto it = m_queues.find(queue);
        it->second->push_back(msg);
    }

private:
};

#endif // _TASK_WORKER_HPP_
