


add_executable(performance)
target_sources(
    performance 
    PRIVATE
    main.cpp
)

target_include_directories(performance PRIVATE ${CMAKE_SOURCE_DIR}/examples/performance)
