configure_file(./config.hpp.in ${PROJECT_SOURCE_DIR}/vvipers/config.hpp)

set(INC_FILES
    Bodypart.hpp
    Collidable.hpp
    CollisionBody.hpp
    CollisionDetector.hpp
    CollisionVertices.hpp
    config.hpp
    debug.hpp
    Controller.hpp
    Game.hpp
    GameEvent.hpp
    GameObject.hpp
    GameWindow.hpp
    Level.hpp
    Math.hpp
    Observer.hpp
    Player.hpp
    RectBody.hpp
    Vec2.hpp
    Viper.hpp
    ViperSketch.hpp
    VVipers.hpp
    Score.hpp
    Time.hpp
    Track.hpp
)

set(SRC_FILES
    Bodypart.cpp
    Collidable.cpp
    CollisionBody.cpp
    CollisionDetector.cpp
    CollisionVertices.cpp
    Controller.cpp
    Game.cpp
    GameEvent.cpp
    GameWindow.cpp
    Level.cpp
    Observer.cpp
    Player.cpp
    RectBody.cpp
    Vec2.cpp
    Viper.cpp
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