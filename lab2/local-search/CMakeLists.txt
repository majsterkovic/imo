cmake_minimum_required(VERSION 3.28)
project(local_search)

set(CMAKE_CXX_STANDARD 23)

add_executable(local_search main.cpp
        Utils.cpp
        Utils.h)
