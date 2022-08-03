#include <fstream>
#include <memory>
#include <vvipers/Engine/Engine.hpp>
#include <vvipers/Engine/FontFileLoader.hpp>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Engine/OptionsJSON.hpp>
#include <vvipers/Engine/Providers.hpp>
#include <vvipers/Engine/TextureFileLoader.hpp>
#include <vvipers/Scenes/FlashScreenScene.hpp>
#include <vvipers/Scenes/MainMenuScene.hpp>
#include <vvipers/Utilities/debug.hpp>
#include <vvipers/config.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

void startGame() {
    std::ifstream cfgFile(CONFIGURATION_FILE_PATH);
    OptionsJSON options(cfgFile);  // The one and only instance
    const std::string resPathOptStr("General/resourceDirectoryPath");
    if (!options.isOptionSet(resPathOptStr))
        options.setOptionString(resPathOptStr, RESOURCE_PATH);
    FontFileLoader fontProvider(&options);
    TextureFileLoader textureProvider(&options);

    auto game = std::make_unique<Game>(options, fontProvider, textureProvider);

    auto firstScene = std::make_shared<FlashScreenScene>(*game.get());
    auto size = game->getWindow().getSize();
    // Center and size in original coordinates
    sf::View menuView(Vec2(0.25*size), 0.5*Vec2(size.x, size.y));
    // Relative position and size in screen coordinates
    menuView.setViewport(sf::FloatRect(0.25, 0.25, 0.5, 0.5));
    auto mainMenu = std::make_shared<MainMenuScene>(*game.get(), menuView);

    Engine engine(std::move(game));
    engine.loadScene(firstScene);
    engine.setDefaultScene(mainMenu);

    engine.startGame();
}

}  // namespace VVipers

int main(int argc, const char** argv) {
    VVipers::debug::verbosity = VVipers::Verbosity::errorsAndWarnings;
    // Handle input arguments?
    VVipers::startGame();
    return 0;
}