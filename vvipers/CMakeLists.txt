configure_file(./config.hpp.in ${PROJECT_BINARY_DIR}/include/vvipers/config.hpp)

set(INC_FILES
    ${PROJECT_BINARY_DIR}/include/vvipers/config.hpp
    Collisions/CollidingBody.hpp
    Collisions/CollisionManager.hpp
    Engine/ColorPalette.hpp
    Engine/Engine.hpp
    Engine/FontFileLoader.hpp
    Engine/GameResources.hpp
    Engine/OptionsJSON.hpp
    Engine/Providers.hpp
    Engine/Scene.hpp
    Engine/TextureFileLoader.hpp
    Engine/WindowManager.hpp
    GameElements/Controller.hpp
    GameElements/FlyingScore.hpp
    GameElements/Food.hpp
    GameElements/GameEvent.hpp
    GameElements/GameObject.hpp
    GameElements/Observer.hpp
    GameElements/Player.hpp
    GameElements/Track.hpp
    GameElements/Viper.hpp
    GameElements/Walls.hpp
    Scenes/ArenaScene.hpp
    Scenes/FlashScreenScene.hpp
    Scenes/GameOverScene.hpp
    Scenes/MainMenuScene.hpp
    Scenes/OptionsMenuScene.hpp
    Scenes/PauseScene.hpp
    Scenes/PlayerConfScene.hpp
    UIElements/MenuButton.hpp
    UIElements/MenuItem.hpp
    UIElements/MenuScene.hpp
    UIElements/PlayerPanel.hpp
    UIElements/ProgressBar.hpp
    UIElements/SelectionButton.hpp
    UIElements/ToggleButton.hpp
    Utilities/debug.hpp
    Utilities/Shape.hpp
    Utilities/Time.hpp
    Utilities/TriangleStripArray.hpp
    Utilities/Vec2.hpp
    Utilities/VVColor.hpp
    Utilities/VVMath.hpp
)

set(SRC_FILES
    Collisions/CollisionManager.cpp
    Engine/ColorPalette.cpp
    Engine/Engine.cpp
    Engine/FontFileLoader.cpp
    Engine/GameResources.cpp
    Engine/OptionsJSON.cpp
    Engine/Scene.cpp
    Engine/TextureFileLoader.cpp
    Engine/WindowManager.cpp
    GameElements/Controller.cpp
    GameElements/FlyingScore.cpp
    GameElements/Food.cpp
    GameElements/GameEvent.cpp
    GameElements/GameObject.cpp
    GameElements/Observer.cpp
    GameElements/Player.cpp
    GameElements/Track.cpp
    GameElements/Viper.cpp
    GameElements/Walls.cpp
    Scenes/ArenaScene.cpp
    Scenes/FlashScreenScene.cpp
    Scenes/GameOverScene.cpp
    Scenes/MainMenuScene.cpp
    Scenes/OptionsMenuScene.cpp
    Scenes/PauseScene.cpp
    Scenes/PlayerConfScene.cpp
    UIElements/MenuButton.cpp
    UIElements/MenuItem.cpp
    UIElements/MenuScene.cpp
    UIElements/PlayerPanel.cpp
    UIElements/ProgressBar.cpp
    UIElements/SelectionButton.cpp
    UIElements/ToggleButton.cpp
    Utilities/Shape.cpp
    Utilities/TriangleStripArray.cpp
    Utilities/Vec2.cpp
)

add_library(libvvipers ${INC_FILES} ${SRC_FILES})

get_target_property(JSONCPP_INCLUDE_DIR jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)

target_include_directories(libvvipers PRIVATE ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR}/include ${JSONCPP_INCLUDE_DIR})
target_link_libraries(libvvipers sfml-graphics jsoncpp_lib)
