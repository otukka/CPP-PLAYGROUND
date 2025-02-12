#include "minitrace.h"

#include <sstream>
#include <iostream>
#include <execinfo.h>
#include <cxxabi.h>
#include <string>
#include <dlfcn.h>
#include <unistd.h>

std::string resolveSym(void* address)
{
    // char** symbols = backtrace_symbols(&address, 1);
    // if (!symbols)
    //     return "Unknown function";

    Dl_info info;
    dladdr(address, &info);
    int status;
    // Demangle the symbol name to make it more readable.
    std::string result;
    char* demangled = abi::__cxa_demangle(info.dli_sname, nullptr, nullptr, &status);
    if (demangled)
    {
        result = demangled;
        free(demangled);
    }
    else
    {

        // result = symbols[0];
        return "Unknown function";
    }

    // Free memory allocated by backtrace_symbols
    // free(symbols);

    return result;
}

std::string backtrace()
{
    void* callstack[128];
    const int nMaxFrames = sizeof(callstack) / sizeof(callstack[0]);
    char buf[1024];
    int nFrames = backtrace(callstack, nMaxFrames);
    char** symbols = backtrace_symbols(callstack, nFrames);

    std::ostringstream trace_buf;
    for (int i = 0; i < nFrames; i++)
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