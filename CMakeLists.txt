cmake_minimum_required(VERSION 3.16)

set(CMAKE_VERBOSE_MAKEFILE ON)

project(app VERSION 1.0.0)

add_executable(${CMAKE_PROJECT_NAME})

target_sources(
    ${CMAKE_PROJECT_NAME} 
    PRIVATE
    src/main.cpp
    src/TaskUsage.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE inc)

add_subdirectory(ThreadSubmodule)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ThreadLibraryExample)


add_subdirectory(tests)