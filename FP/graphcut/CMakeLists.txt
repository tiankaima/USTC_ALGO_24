cmake_minimum_required(VERSION 3.28)
project(GraphCut)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(GraphCut grabcut.cpp
        maxflow/maxflow.cpp
        maxflow/graph.cpp)
target_link_libraries(GraphCut ${OpenCV_LIBS})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/strawberry.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
