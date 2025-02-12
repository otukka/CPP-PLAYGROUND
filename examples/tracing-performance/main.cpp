#include <iostream>
#include <chrono>
#include <vector>
#include <atomic>
#include <cassert>
#include <functional>
#include <complex>
#include <type_traits>
#include <iomanip>

#include "minitrace.h"
#include "tracer.hpp"

int performance_tests();
std::string backtrace();
std::string resolveSym(void* address);

template<typename Func, typename... Args>
void measureExecutionTime(Func&& func, int totalRuns, int skipCount, Args&&... args)
{
    std::vector<double> durations;

    for (int i = 0; i < totalRuns; ++i)
    {
        auto start = std::chrono::high_resolution_clock::now();

        // Call the function with its arguments
        std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        if (i < skipCount)
        {
            // Optional: print skipped runs if needed for debugging
            // std::cout << "Execution " << (i + 1) << " time: " << duration.count() << " seconds\n";
        }
        else
        {
            durations.push_back(duration.count());
        }
    }

    if (!durations.empty())
    {
        double sum = 0;
        for (const auto& d : durations)
        {
            sum += d;
        }

        double average = (sum / durations.size()) * 1e6;  // Convert to microseconds
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "Average execution time: " << average << " us\n";
    }
}

void minitrace_start_stop()
{
    mtr_init("/tmp/trace.json");
    mtr_shutdown();
}

void tracer_start_stop()
{
    TRACE_START("/tmp/trace.json");
    TRACE_STOP();
}

void sym()
{
    resolveSym(reinterpret_cast<void*>(&backtrace));
}

void minitrace_tracer()
{
    MTR_BEGIN("cat", "name");
    MTR_END("cat", "name");
}
void trace_tracer()
{
    TRACE_BEGIN("cat", "name");
    TRACE_END("cat", "name");
}

int main()
{
    int totalRuns = 1000;  // Total number of times to run the function
    int skipCount = 10;    // Number of executions to skip for hot start

    // Measure execution time of the example function
    measureExecutionTime([]() { return 0; }, totalRuns, skipCount);
    measureExecutionTime(backtrace, totalRuns, skipCount);
    measureExecutionTime(sym, totalRuns, skipCount);

    std::cout << "\n" << std::endl;

    measureExecutionTime(minitrace_start_stop, 1, 0);
    measureExecutionTime(minitrace_start_stop, totalRuns, skipCount);

    mtr_init("/tmp/trace1.json");
    measureExecutionTime([]() { MTR_SCOPE_FUNC(); }, 5, 0);
    mtr_shutdown();

    mtr_init("/tmp/trace3.json");
    measureExecutionTime(minitrace_tracer, 5, 0);
    mtr_shutdown();

    std::cout << "\n" << std::endl;

    measureExecutionTime(tracer_start_stop, 1, 0);
    measureExecutionTime(tracer_start_stop, totalRuns, skipCount);

    TRACE_START("/tmp/trace0.json");
    measureExecutionTime([]() { TRACE_SCOPE_AUTO(); }, 5, 0);
    TRACE_STOP();

    TRACE_START("/tmp/trace2.json");
    measureExecutionTime(trace_tracer, 5, 0);
    TRACE_STOP();

    return 0;
}
