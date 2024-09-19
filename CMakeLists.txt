cmake_minimum_required(VERSION 3.28)

project(shortTransform VERSION 0.1 LANGUAGES CXX)

# Set C++ standart
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# Set C++ compile flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "-g -Og -Wall -Wextra")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -Werror")
else()
    message(FATAL_ERROR "(${PROJECT_NAME}) Invalid CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
endif()

# Print configs to console
message("(${PROJECT_NAME}) CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message("(${PROJECT_NAME}) CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

# Set source files
set(SOURCES source/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})