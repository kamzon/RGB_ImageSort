cmake_minimum_required(VERSION 3.3)

# Project name
project(imgSort)

# Support for C++11 is required
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Executable
add_executable(imgsort src/main.cpp src/bmp_io.cpp)