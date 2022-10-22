



# Ctest / catch2 
add_subdirectory(Catch2)
add_executable(tests)

target_sources(tests PRIVATE
main.cpp
ConstructorClassTests.cpp
BitOperationsTests.cpp
../src/Ringbuffer.c
RingbufferTests.cpp
TwosComplementTests.cpp
)

target_include_directories(tests PRIVATE ../inc)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/tests/Catch2/contrib/")

include(CTest)
include(Catch)
catch_discover_tests(tests)