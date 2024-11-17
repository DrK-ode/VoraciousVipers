#pragma once

#include <SFML/Graphics/Color.hpp>
#include <string>
#include <vector>
#include <vvipers/Utilities/Vec2.hpp>

namespace sf {
class Font;
class Texture;
}  // namespace sf

namespace VVipers {

class ColorProvider {
  public:
    virtual ~ColorProvider() {}
    virtual sf::Color get_color(size_t index) const = 0;
};

class FontProvider {
  public:
    virtual ~FontProvider() {}
    virtual const sf::Font* default_font() const = 0;
    /** @returns default font if the wanted font is not found **/
    virtual const sf::Font* font(const std::string& fontname) const = 0;
};

class OptionsProvider {
  public:
    virtual ~OptionsProvider() {}
    virtual bool option_boolean(const std::string& optionName) const = 0;
    virtual std::string option_string(const std::string& optionName) const = 0;
    virtual double option_double(const std::string& optionName) const = 0;
    virtual Vec2 option_2d_vector(const std::string& optionName) const = 0;
    virtual std::vector<bool> option_boolean_array(
        const std::string& optionName) const = 0;
    virtual std::vector<double> option_double_array(
        const std::string& optionName) const = 0;
    virtual std::vector<std::string> option_string_array(
        const std::string& optionName) const = 0;
    virtual std::vector<Vec2> option_2d_vector_array(
        const std::string& optionName) const = 0;
    virtual bool is_option_set(const std::string& optionName) const = 0;
    virtual void write(std::ostream& output) const = 0;

    virtual void set_option_boolean(const std::string& optionName,
                                    bool optionValue) = 0;
    virtual void set_option_double(const std::string& optionName,
                                   double optionValue) = 0;
    virtual void set_option_string(const std::string& optionName,
                                   const std::string& optionValue) = 0;
    virtual void set_option_2d_vector(const std::string& optionName,
                                      Vec2 value) = 0;
    virtual void set_option_boolean_array(
        const std::string& optionName,
        const std::vector<bool>& booleanArray) = 0;
    virtual void set_option_double_array(
        const std::string& optionName,
        const std::vector<double>& doubleArray) = 0;
    virtual void set_option_string_array(
        const std::string& optionName,
        const std::vector<std::string>& stringArray) = 0;
    virtual void set_option_2d_vector_array(
        const std::string& optionName,
        const std::vector<Vec2>& vectorArray) = 0;
};

class TextureProvider {
  public:
    virtual ~TextureProvider() {}
    virtual const sf::Texture* texture(
        const std::string& texturename) const = 0;
};

}  // namespace VVipers
