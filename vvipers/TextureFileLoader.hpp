#ifndef VVIPERS_TEXTUREFILELOADER_HPP
#define VVIPERS_TEXTUREFILELOADER_HPP

#include <map>
#include <vvipers/Services.hpp>

namespace VVipers {

class GameOptions;

class TextureFileLoader : public TextureProvider {
  public:
    TextureFileLoader(const OptionsProvider* options);
    ~TextureFileLoader();
    const sf::Texture* getTexture(
        const std::string& texturename) const override;
    const sf::Texture* loadTexture(const std::string& filename, sf::FloatRect,
                                   bool repeatable);

  private:
    std::string m_resourceDirectoryPath;
    std::map<const std::string, const sf::Texture*> m_textures;
    const sf::Texture* m_defaultTexture;
};

}  // namespace VVipers

#endif  // VVIPERS_TEXTUREFILELOADER_HPP
