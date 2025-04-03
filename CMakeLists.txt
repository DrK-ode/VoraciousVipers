cmake_minimum_required(VERSION 3.10)
project(VoraciousVipers CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_DOCS "Build documentation" ON)
option(BUILD_TESTS "Build tests" ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug
    CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(SFML COMPONENTS system window graphics network audio REQUIRED)
find_package(jsoncpp REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Werror")

add_subdirectory(./vvipers)
if(BUILD_TESTS)
  # Tests depend on library built above (subdir)
  add_subdirectory(./tests)
endif(BUILD_TESTS)

if( BUILD_DOCS )
  add_subdirectory(./docs)
endif( BUILD_DOCS )

add_executable(voraciousvipers "./vvipers/main.cpp")
target_include_directories(voraciousvipers PRIVATE "./" ${PROJECT_BINARY_DIR}/include/)
target_link_libraries(voraciousvipers libvvipers)

install(TARGETS voraciousvipers DESTINATION ~/bin)
