cmake_minimum_required(VERSION 3.28)
project(GrabCut)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(GrabCut grabcut.cpp
        maxflow/maxflow.cpp
        maxflow/graph.cpp
        GMM.cpp)
target_link_libraries(GrabCut ${OpenCV_LIBS})

# copy image to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/input.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/mask.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
