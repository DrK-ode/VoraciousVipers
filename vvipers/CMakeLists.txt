configure_file(./config.hpp.in ${PROJECT_SOURCE_DIR}/vvipers/config.hpp)

set(INC_FILES
    config.hpp
    Engine/Engine.hpp
    Engine/FontFileLoader.hpp
    Engine/Game.hpp
    Engine/OptionsJSON.hpp
    Engine/Providers.hpp
    Engine/TextureFileLoader.hpp
    Scenes/Arena.hpp
    Scenes/FlashScreen.hpp
    Scenes/GameOverScreen.hpp
    Scenes/PauseScreen.hpp
    Scenes/Scene.hpp
    Scenes/Collision/Bodypart.hpp
    Scenes/Collision/Collidable.hpp
    Scenes/Collision/CollisionBody.hpp
    Scenes/Collision/CollisionDetector.hpp
    Scenes/Collision/CollisionVertices.hpp
    Scenes/Collision/ConvexBody.hpp
    Scenes/GameElements/Controller.hpp
    Scenes/GameElements/FlyingScore.hpp
    Scenes/GameElements/Food.hpp
    Scenes/GameElements/GameEvent.hpp
    Scenes/GameElements/GameObject.hpp
    Scenes/GameElements/Observer.hpp
    Scenes/GameElements/Player.hpp
    Scenes/GameElements/PlayerPanel.hpp
    Scenes/GameElements/ProgressBar.hpp
    Scenes/GameElements/Track.hpp
    Scenes/GameElements/Viper.hpp
    Scenes/GameElements/Walls.hpp
    Utilities/debug.hpp
    Utilities/Time.hpp
    Utilities/Vec2.hpp
    Utilities/VVMath.hpp
)

set(SRC_FILES
    Engine/Engine.cpp
    Engine/FontFileLoader.cpp
    Engine/Game.cpp
    Engine/OptionsJSON.cpp
    Engine/TextureFileLoader.cpp
    Scenes/Arena.cpp
    Scenes/FlashScreen.cpp
    Scenes/GameOverScreen.cpp
    Scenes/PauseScreen.cpp
    Scenes/Scene.cpp
    Scenes/Collision/Bodypart.cpp
    Scenes/Collision/Collidable.cpp
    Scenes/Collision/CollisionBody.cpp
    Scenes/Collision/CollisionDetector.cpp
    Scenes/Collision/CollisionVertices.cpp
    Scenes/Collision/ConvexBody.cpp
    Scenes/GameElements/Controller.cpp
    Scenes/GameElements/FlyingScore.cpp
    Scenes/GameElements/Food.cpp
    Scenes/GameElements/GameEvent.cpp
    Scenes/GameElements/GameObject.cpp
    Scenes/GameElements/Observer.cpp
    Scenes/GameElements/Player.cpp
    Scenes/GameElements/PlayerPanel.cpp
    Scenes/GameElements/ProgressBar.cpp
    Scenes/GameElements/Track.cpp
    Scenes/GameElements/Viper.cpp
    Scenes/GameElements/Walls.cpp
    Utilities/Vec2.cpp
)

add_library(libvvipers ${INC_FILES} ${SRC_FILES})
target_include_directories(libvvipers PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(libvvipers sfml-graphics jsoncpp)