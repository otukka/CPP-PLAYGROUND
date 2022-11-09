
#include "TaskExample.hpp"

#include <chrono>
#include <thread>
#include <string>

void run()
{
    TaskExample T {};
    T.init();

    // T.createQueue("test", 600);
    T.createQueue("test", 0);

    T.run();

    std::this_thread::sleep_for(std::literals::chrono_literals::operator""ms(2500));
    for (size_t i = 0; i < 10; i++)
    {
        T.pushMessage("test", "msg form another thread " + std::to_string(i));
    }
    std::this_thread::sleep_for(std::literals::chrono_literals::operator""ms(2500));
    for (size_t i = 10; i < 15; i++)
    {
        T.pushMessage("test", "msg form another thread " + std::to_string(i));
    }

    std::this_thread::sleep_for(std::literals::chrono_literals::operator""ms(2500));

    T.stop();
}