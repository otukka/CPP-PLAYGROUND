
#ifndef _TASK_WORKER_HPP_
#define _TASK_WORKER_HPP_

#include <memory>
#include <thread>
#include <atomic>
#include "MessageQueue.hpp"

class TaskWorker
{
private:

protected:
    std::string m_name;
    std::thread m_thread;
    std::atomic<bool> m_running;
    std::shared_ptr<MessageQueue<std::string>> m_queue;

public:

    TaskWorker(const std::string& name) : m_name { name }, m_running { false } {};

    TaskWorker() = delete;
    TaskWorker(const TaskWorker &other) = delete;
    TaskWorker(const TaskWorker &&other) = delete;
    TaskWorker& operator=(const TaskWorker &&other) = delete;

    virtual bool init() = 0;
    virtual bool run() = 0;
    virtual bool stop() = 0;

    void createQueue(const uint32_t timeout)
    {
        m_queue = std::make_shared<MessageQueue<std::string>>(timeout);
    }

    void pushMessage(const std::string& msg)
    {
        m_queue->push_back(msg);
    }
};

#endif // _TASK_WORKER_HPP_
