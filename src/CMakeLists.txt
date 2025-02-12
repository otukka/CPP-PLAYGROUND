
add_library(Functionality OBJECT)

target_sources(Functionality PRIVATE
    TaskUsage.cpp
    Ringbuffer.c
    # stringview.cpp
)





target_include_directories(Functionality PUBLIC ../inc)

target_compile_options(Functionality PRIVATE -Wall -Wextra -pedantic -Werror -Wconversion)

target_link_libraries(Functionality PRIVATE ThreadLibraryExample)


add_executable(App)

target_sources(
    App 
    PRIVATE
    main.cpp
)

target_link_libraries(App PRIVATE Functionality)

target_compile_options(App PRIVATE -Wall -Wextra -pedantic -Werror -Wconversion)
