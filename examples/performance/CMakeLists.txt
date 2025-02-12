


add_library(functions SHARED)
target_sources(functions PRIVATE functionality.cpp)
# target_compile_options(functions PRIVATE -finstrument-functions)


add_executable(performance)
target_sources(
    performance 
    PRIVATE
    main.cpp
    inst.cpp
    tracer.cpp
)

target_link_libraries(performance PUBLIC functions)
target_include_directories(performance PRIVATE ${CMAKE_SOURCE_DIR}/examples/performance)
# target_compile_options(performance PRIVATE -finstrument-functions -finstrument-functions-exclude-file-list=/usr/lib,/usr/include,/usr/include/c++,minitrace.c,minitrace.h,inst.cpp,main.cpp,tracer.cpp,tracer.hpp)
