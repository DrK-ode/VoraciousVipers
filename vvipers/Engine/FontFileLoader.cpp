#include <SFML/Graphics/Font.hpp>
#include <sstream>
#include <vvipers/Engine/FontFileLoader.hpp>

namespace VVipers {

FontFileLoader::FontFileLoader(const OptionsProvider& options)
    : _default_font(nullptr) {
    _resource_directory_path =
        options.option_string("General/resourceDirectoryPath");
    for (auto& fontname : options.option_string_array("General/fonts"))
        load_font(fontname);
    auto defaultFont = options.option_string("General/defaultFont");
    _default_font = font(defaultFont);
}

FontFileLoader::~FontFileLoader() {
    for (auto f : _fonts)
        delete f.second;
}

const sf::Font* FontFileLoader::default_font() const { return _default_font; }

const sf::Font* FontFileLoader::font(const std::string& fontname) const {
    if (_fonts.contains(fontname))
        return _fonts.at(fontname);
    return _default_font;
}

const sf::Font* FontFileLoader::load_font(const std::string& fontname) {
    std::stringstream fontpath;
    fontpath << _resource_directory_path << fontname << ".ttf";
    sf::Font* font = new sf::Font;
    if (font->loadFromFile(fontpath.str()))
        _fonts[fontname] = font;
    else {
        delete font;
        font = nullptr;
    }
    return font;
}

}  // namespace VVipers
