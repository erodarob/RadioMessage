cmake_minimum_required(VERSION 3.20)
project(RadioMessage-gcc)

include(${CMAKE_CURRENT_LIST_DIR}/../CMakeLists.txt)

add_executable(RadioMessage-gcc main.cpp)
set(CMAKE_CXX_FLAGS "-g")
target_link_libraries(RadioMessage-gcc RadioMessage)