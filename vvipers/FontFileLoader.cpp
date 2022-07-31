#include <sstream>
#include <vvipers/FontFileLoader.hpp>

namespace VVipers {

FontFileLoader::FontFileLoader(const OptionsProvider* options)
    : m_defaultFont(nullptr) {
    m_resourceDirectoryPath =
        options->getOptionString("General/resourceDirectoryPath");
    for (auto& fontname : options->getOptionStringArray("General/fonts"))
        loadFont(fontname);
    auto defaultFont = options->getOptionString("General/defaultFont");
    m_defaultFont = getFont(defaultFont);
}

FontFileLoader::~FontFileLoader() {
    for (auto f : m_fonts)
        delete f.second;
}

const sf::Font* FontFileLoader::getDefaultFont() const { return m_defaultFont; }

const sf::Font* FontFileLoader::getFont(const std::string& fontname) const {
    if (m_fonts.contains(fontname))
        return m_fonts.at(fontname);
    return m_defaultFont;
}

const sf::Font* FontFileLoader::loadFont(const std::string& fontname) {
    std::stringstream fontpath;
    fontpath << m_resourceDirectoryPath << fontname << ".ttf";
    sf::Font* font = new sf::Font;
    if (font->loadFromFile(fontpath.str()))
        m_fonts[fontname] = font;
    else {
        delete font;
        font = nullptr;
    }
    return font;
}

}  // namespace VVipers
