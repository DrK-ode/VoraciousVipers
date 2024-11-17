#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <sstream>
#include <vvipers/Engine/TextureFileLoader.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

TextureFileLoader::TextureFileLoader(const OptionsProvider& options) {
    auto resourceDirectoryPath =
        options.option_string("General/resourceDirectoryPath");

    std::stringstream ss;
    for (auto viperPart : {"ViperHead", "ViperBody", "ViperTail"}) {
        ss.str("");
        ss << "ViperModel/" << viperPart << "/textureFile";
        auto filename = options.option_string(ss.str());
        ss.str("");
        ss << "ViperModel/" << viperPart << "/textureCrop";
        auto cropRect = options.option_double_array(ss.str());
        if (cropRect.size() != 4)
            throw std::runtime_error("cropRect is of the wrong size.");
        ss.str("");
        ss << "ViperModel/" << viperPart << "/repeated";
        auto repeated = options.option_boolean(ss.str());

        ss.str("");
        ss << resourceDirectoryPath << filename;
        sf::Texture* texture = new sf::Texture;
        sf::IntRect area(cropRect[0], cropRect[1], cropRect[2], cropRect[3]);
        texture->loadFromFile(ss.str(), area);
        texture->setRepeated(repeated);

        _textures[viperPart] = texture;
    }
}

TextureFileLoader::~TextureFileLoader() {}

const sf::Texture* TextureFileLoader::texture(
    const std::string& texturename) const {
    if (_textures.contains(texturename))
        return _textures.at(texturename);
    return nullptr;
}

const sf::Texture* TextureFileLoader::load_texture(const std::string& filename,
                                                  sf::FloatRect,
                                                  bool repeatable) {
    return nullptr;
}

}  // namespace VVipers
