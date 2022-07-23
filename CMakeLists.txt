cmake_minimum_required(VERSION 3.10)
project(VoraciousVipers CXX)

option(BUILD_DOCS "Build documentation" ON)
option(BUILD_TESTS "Build tests" ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug
    CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(./vvipers)
if(BUILD_TESTS)
  # Tests depend on library built by the above subdir
  add_subdirectory(./tests)
endif(BUILD_TESTS)

if( BUILD_DOCS )
  add_subdirectory(./docs)
endif( BUILD_DOCS )

add_executable(voraciousvipers "./vvipers/main.cpp")
target_include_directories(voraciousvipers PRIVATE "./")
target_link_libraries(voraciousvipers libvvipers)

install(TARGETS voraciousvipers DESTINATION ~/bin)