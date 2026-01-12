cmake_minimum_required(VERSION 3.20)
project(Message-gcc)

# manually copy main CMake here to make things easier
include(${CMAKE_CURRENT_LIST_DIR}/../CMakeLists.txt)

add_executable(Message-gcc main.cpp)
set(CMAKE_CXX_FLAGS "-g")
target_link_libraries(Message-gcc Message)