

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

add_library(ThreadLibraryExample SHARED)

target_sources(
    ThreadLibraryExample 
    PRIVATE
    src/TaskExample.cpp
)

set(JSON_Install OFF CACHE INTERNAL "")

add_subdirectory(json)

target_link_libraries(ThreadLibraryExample PRIVATE nlohmann_json::nlohmann_json Threads::Threads)

target_include_directories(
    ThreadLibraryExample
    PUBLIC
    inc
)
