cmake_minimum_required(VERSION 4.0)
project(SWE_P3 C)

set(CMAKE_C_STANDARD 23)

add_executable(SWE_P3 main.c
        kanban.h
        kanban.c
        edit.c)
