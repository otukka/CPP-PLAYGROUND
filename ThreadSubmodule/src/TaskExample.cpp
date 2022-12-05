
#include <iostream>
#include <chrono>
#include <string>
#include <fstream>
#include <cstring>
#include <errno.h>
#include <stdio.h>
#include <array>
#include <sstream>
#include <regex>

#include "TaskExample.hpp"

#include "nlohmann/json.hpp"

TaskExample::TaskExample(const std::string& name) : TaskWorker(name) {}

std::string datetime()
{
    const auto now = std::chrono::system_clock::now();
    const auto nowAsTimeT = std::chrono::system_clock::to_time_t(now);
    const auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::stringstream nowSs;
    nowSs << std::put_time(std::localtime(&nowAsTimeT), "%Y-%m-%d %T") << '.' << std::setfill('0') << std::setw(3)
          << nowMs.count();
    return nowSs.str();
}

bool TaskExample::init()
{
    return true;
}

bool TaskExample::run()
{
    m_thread = std::thread(&TaskExample::loop, this);
    return true;
}

void TaskExample::loop()
{
    this->m_running = true;

    while (m_running == true)
    {

        std::string msg{};
        bool status = m_queue->pop_front(msg);

        // Message received otherwise timeout.
        if (status == true)
        {
            std::cout << datetime() << " [msg] " << msg << std::endl;
        }
    }

    return;
}

bool TaskExample::stop()
{
    this->m_running = false;
    this->pushMessage("stop");
    this->m_thread.join();
    return true;
}
