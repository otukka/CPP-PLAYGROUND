cmake_minimum_required (VERSION 3.22.1)

project(app VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_VERBOSE_MAKEFILE ON)

set (CMAKE_CXX_STANDARD 20)


option(ENABLE_CPPCHECK "" FALSE)

if(ENABLE_CPPCHECK)
  find_program(CPPCHECK cppcheck)
  if(CPPCHECK)
    set(CMAKE_CXX_CPPCHECK
        ${CPPCHECK}
        --suppress=unmatchedSuppression:{}
        --suppress=missingIncludeSystem
        --suppress=syntaxError
        --suppress=ConfigurationNotChecked
        --enable=warning
        --inconclusive
        --force
        --xml
        --xml-version=2)
  else()
    message(SEND_ERROR "cppcheck requested but executable not found")
  endif()
endif()

add_subdirectory(ThreadSubmodule)

add_subdirectory(src)

add_subdirectory(Catch2)

add_subdirectory(tests)

add_subdirectory(examples)

enable_testing()