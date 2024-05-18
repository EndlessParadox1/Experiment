cmake_minimum_required(VERSION 3.26)
project(concurrency)

set(CMAKE_CXX_STANDARD 17)

add_executable(concurrency main.cpp
        mySort.cpp
        mySort.h
        mySem.cpp
        mySem.h
        my_rwlock.cpp
        my_rwlock.h
)
