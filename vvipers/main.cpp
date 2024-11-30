#include <memory>
#include <vvipers/Engine/Engine.hpp>
#include <vvipers/Engine/FontFileLoader.hpp>
#include <vvipers/Engine/GameResources.hpp>
#include <vvipers/Engine/OptionsJSON.hpp>
#include <vvipers/Engine/Providers.hpp>
#include <vvipers/Engine/TextureFileLoader.hpp>
#include <vvipers/Scenes/FlashScreenScene.hpp>
#include <vvipers/Scenes/MainMenuScene.hpp>
#include <vvipers/Utilities/debug.hpp>
#include <vvipers/config.hpp>

namespace VVipers {

void startGame() {
    auto options =
        std::make_unique<OptionsJSON>(CONFIGURATION_FILE_PATH);  // The one and only instance
    const std::string resPathOptStr("General/resourceDirectoryPath");
    if (!options->is_option_set(resPathOptStr))
        options->set_option_string(resPathOptStr, RESOURCE_PATH);
    auto game_resources = std::make_unique<GameResources>(std::move(options));

    Engine engine(std::move(game_resources));
    engine.add_scene(std::make_shared<FlashScreenScene>(engine.game_resources()));
    engine.set_default_scene(std::make_shared<MainMenuScene>(engine.game_resources()));

    engine.start_game();
}

}  // namespace VVipers

int main(int argc, const char** argv) {
    VVipers::debug::verbosity = VVipers::Verbosity::ErrorsAndWarnings;
    //VVipers::debug::verbosity = VVipers::Verbosity::All;
    // Handle input arguments?
    VVipers::startGame();
    return 0;
}