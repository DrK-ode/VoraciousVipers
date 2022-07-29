configure_file(./config.hpp.in ${PROJECT_SOURCE_DIR}/vvipers/config.hpp)

set(INC_FILES
    Bodypart.hpp
    Collidable.hpp
    CollisionBody.hpp
    CollisionDetector.hpp
    CollisionVertices.hpp
    config.hpp
    Controller.hpp
    ConvexBody.hpp
    debug.hpp
    FlyingScore.hpp
    FontFileLoader.hpp
    Food.hpp
    Game.hpp
    GameEvent.hpp
    GameObject.hpp
    Observer.hpp
    OptionsJSON.hpp
    Player.hpp
    PlayerPanel.hpp
    ProgressBar.hpp
    Services.hpp
    TextureFileLoader.hpp
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
    FlyingScore.cpp
    FontFileLoader.cpp
    Food.cpp
    Game.cpp
    GameEvent.cpp
    GameObject.cpp
    Observer.cpp
    OptionsJSON.cpp
    Player.cpp
    PlayerPanel.cpp
    ProgressBar.cpp
    Services.cpp
    TextureFileLoader.cpp
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