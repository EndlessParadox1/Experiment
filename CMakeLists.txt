cmake_minimum_required(VERSION 3.29)
project(Experiment)

set(CMAKE_CXX_STANDARD 14)

include_directories(concurrency)

add_executable(Experiment
    async/main.cpp
    bucket/go.mod
    bucket/main.go
    concurrency/main.cpp
    concurrency/my_rwlock.cpp
    concurrency/my_rwlock.h
    concurrency/mySem.cpp
    concurrency/mySem.h
    concurrency/mySort.cpp
    concurrency/mySort.h
    IPC/socket/client.c
    IPC/socket/server.c
    IPC/msg.c
    IPC/pipe.c
    IPC/sema.c
    IPC/shared_mem.c
    IPC/sock_pair.c
    MultiPlex/epoll.cpp
    sys/daemon.c)
