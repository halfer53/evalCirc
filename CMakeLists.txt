cmake_minimum_required(VERSION 3.7)
project(evalCirc)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        bpw_io.c
        bpw_io.h
        evalcirc.c
        evalcirc.h
        evalcircuit.c
        getgate.c
        getgate.h
        randomrd.c
        randomrd.h
        debug.c
        xtrapbits.h bpw_linux.h)

find_library(rt_lib rt)

SET( CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -mrdrnd" )

add_executable(evalCirc ${SOURCE_FILES})

target_link_libraries(evalCirc rt)
