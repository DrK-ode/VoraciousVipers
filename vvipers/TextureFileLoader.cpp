#include <vvipers/TextureFileLoader.hpp>

namespace VVipers {

TextureFileLoader::TextureFileLoader(const OptionsProvider* options) {
    m_resourceDirectoryPath =
        options->getOptionString("General/resourceDirectoryPath");
    // for ( auto& texturename :
    // options->getOptionStringArray("General/textures") ) loadTexture();
}

TextureFileLoader::~TextureFileLoader() {}

const sf::Texture* TextureFileLoader::getTexture(
    const std::string& texturename) const {return nullptr;}

const sf::Texture* TextureFileLoader::loadTexture(const std::string& filename,
                                                  sf::FloatRect,
                                                  bool repeatable) {return nullptr;}

}  // namespace VVipers
