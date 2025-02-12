#include "measure.hpp"

#include <atomic>
#include <cassert>
#include <complex>
#include <cxxabi.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <functional>
#include <mutex>
#include <numeric>
#include <string>
#include <thread>
#include <type_traits>
#include <typeinfo>
#include <unistd.h>
#include <vector>

using fun_t = int(int, int);

// inlineable direct call
int funA(int a, int b)
{
    return a + b;
}

// non-inlined direct call
__attribute__((noinline)) int funB(int a, int b)
{
    return a + b;
}

// non-inlined indirect call
fun_t* funC = funA;  // set externally to funA

// visible lambda
auto funD = [](int a, int b) { return a + b; };

// std::function with visible function
auto funE = std::function<fun_t>(funA);

// std::function with non-inlined function
auto funF = std::function<fun_t>(funB);

// std::function with function pointer
auto funG = std::function<fun_t>(funC);

// std::function with visible lambda
auto funH = std::function<fun_t>(funD);

// std::function with direct lambda
auto funI = std::function<fun_t>([](int a, int b) { return a + b; });

std::function<int(int, int)> f;

int performance_tests()
{

    int totalRuns = 10;  // Total number of times to run the function
    int skipCount = 0;   // Number of executions to skip for hot start

    std::atomic<int> a{105};
    std::atomic<int> b{69};
    int c = a + b;

    // Measure execution time of the example function
    measureExecutionTime(funA, totalRuns, skipCount, c, a, b);
    measureExecutionTime(funB, totalRuns, skipCount, c, a, b);
    measureExecutionTime(funC, totalRuns, skipCount, c, a, b);
    measureExecutionTime(funD, totalRuns, skipCount, c, a, b);
    measureExecutionTime(funE, totalRuns, skipCount, c, a, b);
    measureExecutionTime(funF, totalRuns, skipCount, c, a, b);
    measureExecutionTime(funG, totalRuns, skipCount, c, a, b);
    measureExecutionTime(funH, totalRuns, skipCount, c, a, b);
    measureExecutionTime(funI, totalRuns, skipCount, c, a, b);

    std::cout << "\n" << std::endl;

    // clang-format off
    measureExecutionTime([&](int x, int y) { f = funA; return f(x,y);}, totalRuns, skipCount, c, a, b);
    measureExecutionTime([&](int x, int y) { f = funB; return f(x,y);}, totalRuns, skipCount, c, a, b);
    measureExecutionTime([&](int x, int y) { f = funC; return f(x,y);}, totalRuns, skipCount, c, a, b);
    measureExecutionTime([&](int x, int y) { f = funD; return f(x,y);}, totalRuns, skipCount, c, a, b);
    measureExecutionTime([&](int x, int y) { f = funE; return f(x,y);}, totalRuns, skipCount, c, a, b);
    measureExecutionTime([&](int x, int y) { f = funF; return f(x,y);}, totalRuns, skipCount, c, a, b);
    measureExecutionTime([&](int x, int y) { f = funG; return f(x,y);}, totalRuns, skipCount, c, a, b);
    measureExecutionTime([&](int x, int y) { f = funH; return f(x,y);}, totalRuns, skipCount, c, a, b);
    measureExecutionTime([&](int x, int y) { f = funI; return f(x,y);}, totalRuns, skipCount, c, a, b);
    // clang-format on

    return 0;
}

int main()
{

    auto r = performance_tests();

    return r;
}
