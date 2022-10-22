#ifndef _SERIAL_TASK_HPP_
#define _SERIAL_TASK_HPP_

#include <string>
#include "TaskWorker.hpp"

class TaskExample : public virtual TaskWorker
{
private:
    int fd;
    void loop();

public:
    TaskExample();
    virtual ~TaskExample() = default;
    virtual bool init();
    virtual bool run();
    virtual bool stop();
};

#endif // !_SERIAL_TASK_HPP_

