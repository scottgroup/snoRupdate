cmake_minimum_required(VERSION 3.10.2)

project(snoRupdate)

set(CMAKE_BUILD_TYPE Debug) # or Release
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)

#Bring the headers into the project
include_directories(include)

#Bring and compile all the .cpp files in the src folder
file(GLOB SOURCES "src/*.cpp")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(snoRupdate ${SOURCES})
