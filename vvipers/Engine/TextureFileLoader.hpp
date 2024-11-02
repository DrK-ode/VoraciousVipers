#ifndef VVIPERS_ENGINE_TEXTUREFILELOADER_HPP
#define VVIPERS_ENGINE_TEXTUREFILELOADER_HPP

#include <map>
#include <vvipers/Engine/Providers.hpp>

namespace VVipers {

class GameOptions;

class TextureFileLoader : public TextureProvider {
  public:
    TextureFileLoader(const OptionsProvider& options);
    ~TextureFileLoader();
    const sf::Texture* getTexture(
        const std::string& texturename) const override;

  private:
    const sf::Texture* loadTexture(const std::string& filename, sf::FloatRect,
                                   bool repeatable);
    std::map<const std::string, const sf::Texture*> m_textures;
};

}  // namespace VVipers

#endif  // VVIPERS_ENGINE_TEXTUREFILELOADER_HPP
