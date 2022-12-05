#ifndef SERIAL_TASK_HPP_
#define SERIAL_TASK_HPP_

#include <string>
#include "TaskWorker.hpp"

class TaskExample : public virtual TaskWorker
{
private:
    int fd;
    void loop();

public:
    explicit TaskExample(const std::string& name);
    bool init() override;
    bool run() override;
    bool stop() override;
};

#endif // !SERIAL_TASK_HPP_
