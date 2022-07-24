configure_file(./config.hpp.in ${PROJECT_SOURCE_DIR}/vvipers/config.hpp)

set(INC_FILES
    Bodypart.hpp
    Collidable.hpp
    CollisionBody.hpp
    CollisionDetector.hpp
    CollisionVertices.hpp
    config.hpp
    Controller.hpp
    debug.hpp
    Game.hpp
    GameConfiguration.hpp
    GameEvent.hpp
    GameObject.hpp
    Level.hpp
    Observer.hpp
    Player.hpp
    RectBody.hpp
    Score.hpp
    Time.hpp
    Track.hpp
    Vec2.hpp
    Viper.hpp
    ViperConfig.hpp
    VVMath.hpp
)

set(SRC_FILES
    Bodypart.cpp
    Collidable.cpp
    CollisionBody.cpp
    CollisionDetector.cpp
    CollisionVertices.cpp
    Controller.cpp
    Game.cpp
    GameConfiguration.cpp
    GameEvent.cpp
    Level.cpp
    Observer.cpp
    Player.cpp
    RectBody.cpp
    Score.cpp
    Track.cpp
    Vec2.cpp
    Viper.cpp
    ViperConfig.cpp
)

find_package(SFML COMPONENTS system window graphics network audio REQUIRED)
set(CMAKE_CXX_STANDARD 20)
add_library(libvvipers ${INC_FILES} ${SRC_FILES})
target_include_directories(libvvipers 
    PRIVATE
    ${PROJECT_SOURCE_DIR}
)
target_link_libraries(libvvipers sfml-graphics jsoncpp)