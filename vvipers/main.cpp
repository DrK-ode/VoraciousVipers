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
    auto options =
        std::make_unique<OptionsJSON>(cfgFile);  // The one and only instance
    const std::string resPathOptStr("General/resourceDirectoryPath");
    if (!options->is_option_set(resPathOptStr))
        options->set_option_string(resPathOptStr, RESOURCE_PATH);

    Engine engine(std::move(options));
    auto firstScene = std::make_shared<FlashScreenScene>(*engine.game());
    auto mainMenu = std::make_shared<MainMenuScene>(*engine.game());
    engine.load_scene(firstScene);
    engine.set_default_scene(mainMenu);

    engine.start_game();
}

}  // namespace VVipers

int main(int argc, const char** argv) {
    // VVipers::debug::verbosity = VVipers::Verbosity::errorsAndWarnings;
    VVipers::debug::verbosity = VVipers::Verbosity::All;
    // Handle input arguments?
    VVipers::startGame();
    return 0;
}