cmake_minimum_required(VERSION 3.10.2)
project(OS_lab_3 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(OS_lab_3 main.c linked_list.c)

