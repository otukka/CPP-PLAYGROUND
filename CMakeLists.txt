cmake_minimum_required(VERSION 3.16)

set(CMAKE_VERBOSE_MAKEFILE ON)

project(app VERSION 1.0.0)

add_executable(${CMAKE_PROJECT_NAME})

target_sources(
    ${CMAKE_PROJECT_NAME} 
    PRIVATE
    src/main.cpp
    # inc/ConstructorClass.hpp
    # inc/Bitoperations.hpp
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE inc)


add_subdirectory(Catch2)
add_executable(tests)

target_sources(tests PRIVATE
tests/main.cpp
tests/ConstructorClassTests.cpp
tests/BitOperationsTests.cpp
src/Ringbuffer.c
tests/RingbufferTests.cpp
tests/TwosComplementTests.cpp
)

target_include_directories(tests PRIVATE inc)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/Catch2/contrib/")

include(CTest)
include(Catch)
catch_discover_tests(tests)