cmake_minimum_required(VERSION 3.30)
project(stribog)

set(CMAKE_CXX_STANDARD 20)
find_package(OpenMP REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fopenmp")

add_executable(stribog main.cpp
        gost341118.h
        gost341118.cpp)


#set_target_properties(stribog PROPERTIES LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fopenmp")
