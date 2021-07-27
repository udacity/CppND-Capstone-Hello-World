cmake_minimum_required(VERSION 3.7)

project(WebCrawler)

add_definitions(-std=c++17)

# Libraries
find_package(CURL REQUIRED)


set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS} -pthread")

# Find all executables
file(GLOB project_SRCS src/*.cpp src/*.h)

add_executable(WebCrawler ${project_SRCS})
target_link_libraries(WebCrawler CURL::libcurl)
