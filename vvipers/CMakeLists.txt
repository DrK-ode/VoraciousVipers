configure_file(./config.hpp.in ${PROJECT_SOURCE_DIR}/vvipers/config.hpp)

set(INC_FILES
    config.hpp
    debug.hpp
    Controller.hpp
    Game.hpp
    GameWindow.hpp
    Level.hpp
    Player.hpp
    Score.hpp
    Track.hpp
    VectorMath.hpp
    Viper.hpp
    ViperVertices.hpp
    VVipers.hpp
)

set(SRC_FILES
    Controller.cpp
    Game.cpp
    GameWindow.cpp
    Level.cpp
    Player.cpp
    Score.cpp
    Track.cpp
    Viper.cpp
    ViperVertices.cpp
    VVipers.cpp
)

find_package(SFML COMPONENTS system window graphics network audio REQUIRED)
set(CMAKE_CXX_STANDARD 20)
add_library(libvvipers ${INC_FILES} ${SRC_FILES})
target_include_directories(libvvipers 
    PRIVATE
    ${PROJECT_SOURCE_DIR}
)
target_link_libraries(libvvipers sfml-graphics)