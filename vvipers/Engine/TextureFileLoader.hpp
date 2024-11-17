#pragma once

#include <map>
#include <vvipers/Engine/Providers.hpp>

namespace VVipers {

class GameOptions;

class TextureFileLoader : public TextureProvider {
  public:
    TextureFileLoader(const OptionsProvider& options);
    ~TextureFileLoader();
    const sf::Texture* texture(const std::string& texturename) const override;

  private:
    const sf::Texture* load_texture(const std::string& filename, sf::FloatRect,
                                    bool repeatable);
    std::map<const std::string, const sf::Texture*> _textures;
};

}  // namespace VVipers
