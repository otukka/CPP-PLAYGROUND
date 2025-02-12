#include "tracer.hpp"

#include <iostream>
#include <execinfo.h>
#include <cxxabi.h>
#include <string>
#include <dlfcn.h>
#include <unistd.h>

namespace
{

std::string resolveSym(void* address)
{

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

    return result;
}
}

extern "C" {

void __cyg_profile_func_enter(void* this_fn, void* call_site)
{
    TRACE_BEGIN("PERF", resolveSym(this_fn));
}

void __cyg_profile_func_exit(void* this_fn, void* call_site)
{
    TRACE_END("PERF", resolveSym(this_fn));
}

}  // extern "C"