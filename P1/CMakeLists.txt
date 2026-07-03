cmake_minimum_required(VERSION 4.0)
project(SWE_P1 C)

set(CMAKE_C_STANDARD 23)

add_executable(SWE_P1 sortAndShuffle/main.c
        sortAndShuffle/sortAndShuffle.c
        sortAndShuffle/sortAndShuffle.h
)
