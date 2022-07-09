configure_file(./config.hpp.in ${PROJECT_SOURCE_DIR}/vvipers/config.hpp)

set(INC_FILES
    Collidable.hpp
    config.hpp
    debug.hpp
    Controller.hpp
    Game.hpp
    GameWindow.hpp
    Level.hpp
    Player.hpp
    Math.hpp
    Vec2.hpp
    Viper.hpp
    ViperGraphics.hpp
    ViperPhysics.hpp
    ViperSketch.hpp
    VVipers.hpp
    Score.hpp
    Time.hpp
    Track.hpp
)

set(SRC_FILES
    Collidable.cpp
    Controller.cpp
    Game.cpp
    GameWindow.cpp
    Level.cpp
    Player.cpp
    Vec2.cpp
    Viper.cpp
    ViperGraphics.cpp
    ViperPhysics.cpp
    ViperSketch.cpp
    VVipers.cpp
    Score.cpp
    Track.cpp
)

find_package(SFML COMPONENTS system window graphics network audio REQUIRED)
set(CMAKE_CXX_STANDARD 20)
add_library(libvvipers ${INC_FILES} ${SRC_FILES})
target_include_directories(libvvipers 
    PRIVATE
    ${PROJECT_SOURCE_DIR}
)
target_link_libraries(libvvipers sfml-graphics)