cmake_minimum_required(VERSION 3.16)
project(ios-proj2 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -Wextra -Werror -pedantic -lpthread")

add_executable(ios-proj2 proj2.c)
target_link_libraries(ios-proj2 pthread)
