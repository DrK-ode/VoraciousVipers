#include <fstream>
#include <vvipers/Arena.hpp>
#include <vvipers/config.hpp>
#include <vvipers/debug.hpp>
#include <vvipers/Engine.hpp>
#include <vvipers/FontFileLoader.hpp>
#include <vvipers/Game.hpp>
#include <vvipers/OptionsJSON.hpp>
#include <vvipers/Providers.hpp>
#include <vvipers/TextureFileLoader.hpp>

namespace VVipers {

void startGame() {
    std::ifstream cfgFile(CONFIGURATION_FILE_PATH);
    OptionsJSON options(cfgFile);  // The one and only instance
    const std::string resPathOptStr("General/resourceDirectoryPath");
    if (!options.isOptionSet(resPathOptStr))
        options.setOptionString(resPathOptStr, RESOURCE_PATH);
    FontFileLoader fontProvider(&options);
    TextureFileLoader textureProvider(&options);
    Engine engine(options,fontProvider,textureProvider);

    engine.startGame();
}

}  // namespace VVipers

int main(int argc, const char** argv) {
    VVipers::debug::verbosity = VVipers::Verbosity::errorsAndWarnings;
    // Handle input arguments?
    VVipers::startGame();
    return 0;
}