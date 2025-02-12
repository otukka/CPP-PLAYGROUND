
add_executable(tests)

target_compile_options(tests PRIVATE -Wall -Wextra -pedantic -Werror -Wconversion)

target_sources(tests PRIVATE
AnonymousStruct.cpp
BitOperationsTests.cpp
ConstructorClassTests.cpp
DestructorTests.cpp
LambdasTests.cpp
main.cpp
PointerTests.cpp
ReferenceTests.cpp
RingbufferTests.cpp
roundTest.cpp
SmartPointerTests.cpp
TemplateTests.cpp
TwosComplementTests.cpp
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain Functionality)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/Catch2/contrib/")

include(CTest)

include(Catch)

catch_discover_tests(tests)
