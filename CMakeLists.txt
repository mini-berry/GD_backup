# project name
PROJECT(gd)
# requirement of cmake version
cmake_minimum_required(VERSION 3.5)

# set the directory of executable files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${gd_SOURCE_DIR}/bin)

# find required opencv
find_package(OpenCV REQUIRED)
# directory of opencv headers
include_directories(${OpenCV_INCLUDE_DIRS} ${gd_SOURCE_DIR}/include)
# name of executable file and path of source file
add_executable(gd src/main.cpp src/user.cpp src/dijkstra.cpp src/BFS.cpp)
# directory of opencv library
link_directories(${OpenCV_LIBRARY_DIRS})
# opencv libraries
target_link_libraries(gd ${OpenCV_LIBS})