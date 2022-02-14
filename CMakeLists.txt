cmake_minimum_required(VERSION 3.20)
project(c_calculator C)

set(CMAKE_C_STANDARD 11)

add_executable(c_calculator main.c functions.c functions.h variables.c variables.h mainMethods.c mainMethods.h)
