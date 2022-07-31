#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <sstream>
#include <vvipers/Engine/TextureFileLoader.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

TextureFileLoader::TextureFileLoader(const OptionsProvider* options) {
    auto resourceDirectoryPath =
        options->getOptionString("General/resourceDirectoryPath");

    std::stringstream ss;
    for (auto viperPart : {"ViperHead", "ViperBody", "ViperTail"}) {
        ss.str("");
        ss << "ViperModel/" << viperPart << "/textureFile";
        auto filename = options->getOptionString(ss.str());
        ss.str("");
        ss << "ViperModel/" << viperPart << "/textureCrop";
        auto cropRect = options->getOptionDoubleArray(ss.str());
        if (cropRect.size() != 4)
            throw std::runtime_error("cropRect is of the wrong size.");
        ss.str("");
        ss << "ViperModel/" << viperPart << "/repeated";
        auto repeated = options->getOptionBoolean(ss.str());

        ss.str("");
        ss << resourceDirectoryPath << filename;
        sf::Texture* texture = new sf::Texture;
        sf::IntRect area(cropRect[0], cropRect[1], cropRect[2], cropRect[3]);
        texture->loadFromFile(ss.str(), area);
        texture->setRepeated(repeated);

        m_textures[viperPart] = texture;
    }
}

TextureFileLoader::~TextureFileLoader() {}

const sf::Texture* TextureFileLoader::getTexture(
    const std::string& texturename) const {
    if (m_textures.contains(texturename))
        return m_textures.at(texturename);
    return nullptr;
}

const sf::Texture* TextureFileLoader::loadTexture(const std::string& filename,
                                                  sf::FloatRect,
                                                  bool repeatable) {
    return nullptr;
}

}  // namespace VVipers
