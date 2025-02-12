
#include "TaskExample.hpp"

#include <chrono>
#include <thread>
#include <string>

using namespace std::chrono_literals;
using millis = std::chrono::duration<uint32_t, std::milli>;

void run()
{

    TaskExample exampleTask("exampleThread");

    if (exampleTask.init() != true)
    {
        return;
    }

    // exampleTask.createQueue(100);
    // exampleTask.createQueue(10000);
    // exampleTask.createQueue(1000);
    exampleTask.createQueue(UINT32_MAX);

    exampleTask.run();

    std::this_thread::sleep_for(millis(50));
    for (size_t i = 0; i < 10; i++)
    {
        exampleTask.pushMessage("msg " + std::to_string(i) + " form main thread ");
        std::this_thread::sleep_for(millis(5));
    }
    std::this_thread::sleep_for(millis(1000));

    exampleTask.stop();
}