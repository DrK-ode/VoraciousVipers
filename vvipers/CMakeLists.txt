configure_file(./config.hpp.in ${PROJECT_SOURCE_DIR}/vvipers/config.hpp)

set(INC_FILES
    Bodypart.hpp
    Collidable.hpp
    CollisionBody.hpp
    CollisionDetector.hpp
    CollisionVertices.hpp
    ConvexBody.hpp
    config.hpp
    Controller.hpp
    debug.hpp
    Food.hpp
    Game.hpp
    GameEvent.hpp
    GameObject.hpp
    GameOptions.hpp
    Observer.hpp
    Player.hpp
    PlayerPanel.hpp
    ProgressBar.hpp
    Time.hpp
    Track.hpp
    Vec2.hpp
    Viper.hpp
    VVMath.hpp
    Walls.hpp
)

set(SRC_FILES
    Bodypart.cpp
    Collidable.cpp
    CollisionBody.cpp
    CollisionDetector.cpp
    CollisionVertices.cpp
    Controller.cpp
    ConvexBody.cpp
    Food.cpp
    Game.cpp
    GameEvent.cpp
    GameObject.cpp
    GameOptions.cpp
    Observer.cpp
    Player.cpp
    PlayerPanel.cpp
    ProgressBar.cpp
    Track.cpp
    Vec2.cpp
    Viper.cpp
    Walls.cpp
)

find_package(SFML COMPONENTS system window graphics network audio REQUIRED)
set(CMAKE_CXX_STANDARD 20)
add_library(libvvipers ${INC_FILES} ${SRC_FILES})
target_include_directories(libvvipers 
    PRIVATE
    ${PROJECT_SOURCE_DIR}
)
target_link_libraries(libvvipers sfml-graphics jsoncpp)