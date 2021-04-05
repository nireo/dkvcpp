cmake_minimum_required(VERSION 3.17)
project(dkvcpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(dkvcpp main.cpp master.cpp  config.cpp config.hpp)