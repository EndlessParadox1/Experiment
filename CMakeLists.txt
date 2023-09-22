cmake_minimum_required(VERSION 3.26)
project(concurrent C)

set(CMAKE_C_STANDARD 11)

add_executable(concurrent main.c
        mySort.c
        mySort.h
        mySem.h
        mySem.c)
