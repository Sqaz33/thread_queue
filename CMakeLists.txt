cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(thread_queue)

file(GLOB_RECURSE INC "include/*.hpp")
file(GLOB_RECURSE SRC "SRC/*.cpp")

add_executable(main ${INC} ${SRC})



