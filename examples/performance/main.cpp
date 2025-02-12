// #include <iostream>
// #include <chrono>
// #include <vector>
// #include <atomic>
// #include <cassert>
// #include <functional>
// #include <complex>
// #include <type_traits>

#include "tracer.hpp"

int performance_tests();
std::string backtrace(int skip = 0);
std::string resolveSym(void* address);
void func();

int main()
{
    std::cout << backtrace() << std::endl;

    TRACE_START("/tmp/trace.json");
    func();

    auto r = performance_tests();
    TRACE_STOP();
    return r;
}
