cmake_minimum_required(VERSION 3.7)
project(evalCirc)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        bpw_io.c
        bpw_io.h
        evalCirc.c
        evalCirc.h
        EvalCircuit.c
        EvalCircuit.h
        getGate.c
        getGate.h
        randomRD.c
        randomRD.h
        xtrapbits.h bpw_linux.h)

add_executable(evalCirc ${SOURCE_FILES})