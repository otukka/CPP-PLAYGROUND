
add_executable(tracing-performance)



target_sources(
    tracing-performance 
    PRIVATE
    main.cpp
    minitrace.h
    minitrace.c
    inst.cpp
)


target_compile_options(tracing-performance PRIVATE -DMTR_ENABLED -DMTR_COPY_EVENT_CATEGORY_AND_NAME)
# target_compile_options(tracing-performance PRIVATE -O0)
target_link_libraries(tracing-performance PUBLIC functions)
