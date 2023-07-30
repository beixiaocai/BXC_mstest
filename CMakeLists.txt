cmake_minimum_required(VERSION 3.10.2)
project(BXC_mstest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_BUILD_TYPE DEBUG)
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(INCLUDE_DIR /usr/local/include)
set(LIB_DIR /usr/local/lib)
include_directories(${INCLUDE_DIR})
link_directories(${LIB_DIR})

add_executable(BXC_mstest
        AvPullStream.cpp
        main.cpp
        )
TARGET_LINK_LIBRARIES(BXC_mstest avformat avcodec avutil swscale swresample pthread)






