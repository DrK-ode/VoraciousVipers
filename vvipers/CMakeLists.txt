configure_file(./config.hpp.in ${PROJECT_SOURCE_DIR}/vvipers/config.hpp)

set(INC_FILES
    Collidable.hpp
    CollisionDetector.hpp
    config.hpp
    debug.hpp
    Controller.hpp
    Game.hpp
    GameEvent.hpp
    GameWindow.hpp
    Level.hpp
    Observer.hpp
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
    CollisionDetector.cpp
    Controller.cpp
    Game.cpp
    GameEvent.cpp
    GameWindow.cpp
    Level.cpp
    Observer.cpp
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