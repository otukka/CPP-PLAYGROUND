
#include "TaskExample.hpp"

#include <chrono>
#include <thread>

void run()
{
    TaskExample T{};
    T.init();
    T.run();

    std::this_thread::sleep_for(std::literals::chrono_literals::operator""s(5));

    T.stop();
}