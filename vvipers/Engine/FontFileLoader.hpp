#ifndef VVIPERS_ENGINE_FONTFILELOADER_HPP
#define VVIPERS_ENGINE_FONTFILELOADER_HPP

#include <map>
#include <string>
#include <vvipers/Engine/Providers.hpp>

namespace VVipers {

class GameOptions;

class FontFileLoader : public FontProvider {
  public:
    FontFileLoader(const OptionsProvider& options);
    ~FontFileLoader();
    const sf::Font* getDefaultFont() const override;
    /** @returns default font if the wanted font is not found **/
    const sf::Font* getFont(const std::string& fontname) const override;
    /** Loads the font if found in the resource folder. Fontname is given
     * without the .ttf suffix **/
    const sf::Font* loadFont(const std::string& fontname);

  private:
    std::string m_resourceDirectoryPath;
    std::map<const std::string, const sf::Font*> m_fonts;
    const sf::Font* m_defaultFont;
};

}  // namespace VVipers

#endif // VVIPERS_ENGINE_FONTFILELOADER_HPP