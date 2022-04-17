cmake_minimum_required(VERSION 3.20)
project(projekt2_ios C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")

add_executable(projekt2_ios proj2.c)
