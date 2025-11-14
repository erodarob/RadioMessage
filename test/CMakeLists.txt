cmake_minimum_required(VERSION 3.20)
project(Message-gcc)

# manually copy main CMake here to make things easier
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src)

add_library(Message STATIC ${LIB_DIR}/Message.cpp ${LIB_DIR}/APRS/APRSCmd.cpp ${LIB_DIR}/APRS/APRSData.cpp ${LIB_DIR}/APRS/APRSTelem.cpp ${LIB_DIR}/APRS/APRSText.cpp ${LIB_DIR}/GroundStation/GSControl.cpp ${LIB_DIR}/GroundStation/GSData.cpp ${LIB_DIR}/GroundStation/Metrics.cpp ${LIB_DIR}/Raw/GenericData.cpp ${LIB_DIR}/Raw/VideoData.cpp ${LIB_DIR}/Types/PackedNum.cpp)
target_include_directories(Message PUBLIC ${LIB_DIR})

add_executable(Message-gcc main.cpp)
set(CMAKE_CXX_FLAGS "-g")
target_link_libraries(Message-gcc Message)