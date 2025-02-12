
add_executable(demangle)
target_sources(
    demangle 
    PRIVATE
    main.cpp

)

target_link_libraries(demangle PUBLIC functions)
target_include_directories(demangle PRIVATE ${CMAKE_SOURCE_DIR}/examples/demangle)

