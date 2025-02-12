#include <iostream>



#include "tracer.hpp"



int main()
{
    std::cout << backtrace() << std::endl;

    TRACE_START("/tmp/trace.json");
    func();

    auto r = performance_tests();
    TRACE_STOP();
    return r;
}
