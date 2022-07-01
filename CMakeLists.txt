cmake_minimum_required(VERSION 3.10)
project(VoraciousVipers CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug
    CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

find_package(SFML COMPONENTS system window graphics network audio REQUIRED)

configure_file(./include/config.hpp.in ${PROJECT_SOURCE_DIR}/include/config.hpp)

set(INC_FILES "")
list(APPEND INC_FILES "include/config.hpp")
list(APPEND INC_FILES "include/debug.hpp")
list(APPEND INC_FILES "include/Game.hpp")
list(APPEND INC_FILES "include/GameManager.hpp")
list(APPEND INC_FILES "include/GameWindow.hpp")
list(APPEND INC_FILES "include/Level.hpp")
list(APPEND INC_FILES "include/Track.hpp")
list(APPEND INC_FILES "include/VectorMath.hpp")
list(APPEND INC_FILES "include/Viper.hpp")

set(SRC_FILES "")
list(APPEND SRC_FILES "src/main.cpp")
list(APPEND SRC_FILES "src/Game.cpp")
list(APPEND SRC_FILES "src/GameManager.cpp")
list(APPEND SRC_FILES "src/GameWindow.cpp")
list(APPEND SRC_FILES "src/Level.cpp")
list(APPEND SRC_FILES "src/Track.cpp")
list(APPEND SRC_FILES "src/Viper.cpp")

add_executable(vorvip ${INC_FILES} ${SRC_FILES})

target_include_directories(vorvip PUBLIC "${PROJECT_SOURCE_DIR}/include")

target_link_libraries(vorvip sfml-graphics)

install(TARGETS vorvip DESTINATION ~/bin)