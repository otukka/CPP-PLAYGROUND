
#include <iostream>
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

TaskExample::TaskExample()
{   

}

std::string datetime()
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, "%Y-%m-%d_%H-%M-%S", timeinfo);
    return std::string(buffer);
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
    this->m_running = 1;

    while(m_running == 1)
    {
        std::cout << datetime() << std::endl;
        std::this_thread::sleep_for(std::literals::chrono_literals::operator""s(1));
    }
    

    return;
}

bool TaskExample::stop()
{   
    this->m_running = 0;
    this->m_thread.join();
    return true;
}

