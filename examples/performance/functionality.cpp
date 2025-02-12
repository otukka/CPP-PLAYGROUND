#include <iostream>
#include <chrono>
#include <vector>
#include <atomic>
#include <cassert>
#include <functional>
#include <complex>
#include <type_traits>

// Partial source: https://artificial-mind.net/blog/2019/09/07/std-function-performance
#include <iostream>
#include <execinfo.h>
#include <cxxabi.h>
#include <string>
#include <unistd.h>
#include <dlfcn.h>
#include <typeinfo>

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <numeric>  // For std::accumulate

#include "digestible.hpp"

namespace
{
std::mutex mutex;

// Function that performs some arbitrary work (e.g., summing up numbers)
void computeSum(int start, int end, int threadId)
{
    std::vector<int> numbers(end - start + 1);
    std::iota(numbers.begin(), numbers.end(), start);  // Fill with values from 'start' to 'end'

    long sum = std::accumulate(numbers.begin(), numbers.end(), 0L);  // Accumulate as long

    std::lock_guard<std::mutex> lock(mutex);
    std::cout << "Thread " << threadId << " calculated sum from " << start << " to " << end << " as: " << sum
              << std::endl;
}

// Function to print a message (arbitrary work)
void printMessage(int threadId, const std::string& message)
{
    std::lock_guard<std::mutex> lock(mutex);
    std::cout << "Thread " << threadId << " says: " << message << std::endl;
}
}
void func()
{
    const int numThreads = 5;  // Number of threads to create
    std::vector<std::thread> threads;

    // Launch threads that perform computation tasks
    for (int i = 0; i < numThreads; ++i)
    {
        threads.emplace_back(computeSum, i * 10, (i + 1) * 10 - 1, i);
    }

    // Launch threads that print messages
    for (int i = 0; i < numThreads; ++i)
    {
        threads.emplace_back(printMessage, i, "Hello from thread!");
    }

    // Wait for all threads to complete
    for (auto& t : threads)
    {
        t.join();
    }

    std::cout << "All threads have completed their work." << std::endl;
}

std::string backtrace(int skip = 0)
{
    void* callstack[128];
    const int nMaxFrames = sizeof(callstack) / sizeof(callstack[0]);
    char buf[1024];
    int nFrames = backtrace(callstack, nMaxFrames);
    char** symbols = backtrace_symbols(callstack, nFrames);

    std::ostringstream trace_buf;
    for (int i = skip; i < nFrames; i++)
    {
        Dl_info info;
        if (dladdr(callstack[i], &info))
        {
            char* demangled = nullptr;
            int status;
            demangled = abi::__cxa_demangle(info.dli_sname, nullptr, 0, &status);

            // Use %p for pointers, %ld for pointer difference
            snprintf(buf, sizeof(buf), "%-3d %*p %s + %ld\n", i, 2 + (int)(sizeof(void*) * 2), callstack[i],
                     status == 0 ? demangled : info.dli_sname, (long)((char*)callstack[i] - (char*)info.dli_saddr));
            free(demangled);
        }
        else
        {
            // Use %p for pointer
            snprintf(buf, sizeof(buf), "%-3d %*p\n", i, 2 + (int)(sizeof(void*) * 2), callstack[i]);
        }
        trace_buf << buf;

        // Print symbol names as strings
        snprintf(buf, sizeof(buf), "%s\n", symbols[i]);
        trace_buf << buf;
    }
    free(symbols);

    if (nFrames == nMaxFrames)
        trace_buf << "[truncated]\n";

    return trace_buf.str();
}

// Template function to measure execution time
template<typename Func, typename... Args>
void measureExecutionTime(Func&& func, int totalRuns, int skipCount, int check, Args&&... args)
{

    // To hold the duration of each run
    std::vector<double> durations;

    for (int i = 0; i < totalRuns; ++i)
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto end = std::chrono::high_resolution_clock::now();

        if constexpr (std::is_void<std::invoke_result<Func, Args...>>{})
        {
            // Call the function with its arguments (for void return type)
            std::forward<Func>(func)(std::forward<Args>(args)...);
            auto end = std::chrono::high_resolution_clock::now();
        }
        else
        {
            // Call the function with its arguments and assign the result (for non-void return type)
            auto result = std::forward<Func>(func)(std::forward<Args>(args)...);
            end = std::chrono::high_resolution_clock::now();
            assert(result == check);
        }

        std::chrono::duration<double> duration = end - start;

        durations.push_back(duration.count());  // Store the duration for average calculation
    }

    // Calculate and report the average time if there are valid runs
    if (!durations.empty())
    {
        digestible::tdigest digest(durations.size());

        for (const auto& d : durations)
        {
            digest.insert(d);
        }
        digest.merge();
        std::cout << "p10: " << std::setprecision(6) << digest.quantile(10) * 1e9 << " nanoseconds";
        std::cout << " p50: " << std::setprecision(6) << digest.quantile(50) * 1e9 << " nanoseconds";
        std::cout << " p99: " << std::setprecision(6) << digest.quantile(99) * 1e9 << " nanoseconds\n";
    }
}

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
