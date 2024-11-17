#pragma once

#include <map>
#include <string>
#include <vvipers/Engine/Providers.hpp>

namespace VVipers {

class GameOptions;

class FontFileLoader : public FontProvider {
  public:
    FontFileLoader(const OptionsProvider& options);
    ~FontFileLoader();
    const sf::Font* default_font() const override;
    /** @returns default font if the wanted font is not found **/
    const sf::Font* font(const std::string& fontname) const override;
    /** Loads the font if found in the resource folder. Fontname is given
     * without the .ttf suffix **/
    const sf::Font* load_font(const std::string& fontname);

  private:
    std::string _resource_directory_path;
    std::map<const std::string, const sf::Font*> _fonts;
    const sf::Font* _default_font;
};

}  // namespace VVipers
