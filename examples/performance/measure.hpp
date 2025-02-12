#include "digestible.hpp"

#include <chrono>
#include <iostream>
#include <iomanip>

// Partial source: https://artificial-mind.net/blog/2019/09/07/std-function-performance

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
        std::cout << std::setprecision(2) << std::fixed;
        std::cout << "p10: " << digest.quantile(10) * 1e9 << " nanoseconds";
        std::cout << " p50: " << digest.quantile(50) * 1e9 << " nanoseconds";
        std::cout << " p99: " << digest.quantile(99) * 1e9 << " nanoseconds\n";
    }
}
