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
    auto mainMenu = std::make_shared<MainMenuScene>(*game.get());

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