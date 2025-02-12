#include <iostream>
#include <chrono>
#include <vector>
#include <atomic>
#include <cassert>
#include <functional>
#include <complex>
#include <type_traits>

#include <iostream>
#include <execinfo.h>
#include <cxxabi.h>
#include <string>
#include <unistd.h>
#include <dlfcn.h>
#include <typeinfo>

// This function produces a stack backtrace with demangled function & method names.
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
            char* demangled = NULL;
            int status;
            demangled = abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);
            snprintf(buf, sizeof(buf), "%-3d %*0p %s + %zd\n", i, 2 + sizeof(void*) * 2, callstack[i],
                     status == 0 ? demangled : info.dli_sname, (char*)callstack[i] - (char*)info.dli_saddr);
            free(demangled);
        }
        else
        {
            snprintf(buf, sizeof(buf), "%-3d %*0p\n", i, 2 + sizeof(void*) * 2, callstack[i]);
        }
        trace_buf << buf;

        snprintf(buf, sizeof(buf), "%s\n", symbols[i]);
        trace_buf << buf;
    }
    free(symbols);
    if (nFrames == nMaxFrames)
        trace_buf << "[truncated]\n";
    return trace_buf.str();
}

void asdf(void* address)
{
    std::cout << "address " << address << std::endl;
    std::string a = "asdgh";
    std::cout << typeid(&a).name() << std::endl;

    void* callstack[128];
    const int nMaxFrames = sizeof(callstack) / sizeof(callstack[0]);
    int nFrames = backtrace(callstack, nMaxFrames);

    for (size_t i = 0; i < nFrames; i++)
    {
        std::cout << callstack[i] << std::endl;
    }

    char** symbols = backtrace_symbols(callstack, nFrames);

    for (size_t i = 0; i < nFrames; i++)
    {
        std::cout << "symbol: " << symbols[i] << std::endl;
    }

    for (size_t i = 0; i < nFrames; i++)
    {
        Dl_info info;
        auto status = dladdr(callstack[i], &info);
        std::cout << "info.dli_fname: " << info.dli_fname << std::endl;
    }
}

// Helper function to get the demangled function name from an address.
std::string get_function_name(void* address)
{

    void* buffer[1]{address};
    char** symbols;

    /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
        would produce similar output to the following: */

    symbols = backtrace_symbols(buffer, 1);
    if (!symbols)
        return "Unknown function";

    std::string result;
    char* demangled = nullptr;
    int status;

    // Print the raw symbol for debugging
    std::cout << "Raw symbol: " << symbols[0] << std::endl;

    // Demangle the symbol, if possible.
    demangled = abi::__cxa_demangle(symbols[0], nullptr, nullptr, &status);
    if (status == 0 && demangled)
    {
        result = demangled;  // Store the demangled name
        free(demangled);     // Free the allocated memory
    }
    else
    {
        std::cerr << "Demangling failed for: " << symbols[0] << std::endl;
        result = symbols[0];  // Use the raw symbol if demangling fails
    }

    free(symbols);  // Free the memory allocated by backtrace_symbols
    return result;
}

using fun_t = int(int, int);

// inlineable direct call
int funA(int a, int b)
{
    return a + b;
}

int main()
{

    // std::cout << backtrace() << std::endl;
    asdf(reinterpret_cast<void*>(&funA));
    std::cout << get_function_name(reinterpret_cast<void*>(&funA)) << std::endl;

    return 0;
}
