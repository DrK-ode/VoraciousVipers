cmake_minimum_required(VERSION 3.10)
project(VoraciousVipers CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug
    CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

add_subdirectory(./vvipers)
# Tests depend on library built by the above subdir
add_subdirectory(./tests)

add_executable(voraciousvipers "./vvipers/VVipers.hpp" "./vvipers/main.cpp")
target_include_directories(voraciousvipers PRIVATE "./")
target_link_libraries(voraciousvipers libvvipers)

install(TARGETS voraciousvipers DESTINATION ~/bin)