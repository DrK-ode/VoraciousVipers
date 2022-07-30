#ifndef VVIPERS_SERVICES_HPP
#define VVIPERS_SERVICES_HPP

#include <string>
#include <vector>
#include <vvipers/Vec2.hpp>

namespace sf {
class Font;
class Texture;
}  // namespace sf

namespace VVipers {

class FontProvider {
  public:
    virtual const sf::Font* getDefaultFont() const = 0;
    /** @returns default font if the wanted font is not found **/
    virtual const sf::Font* getFont(const std::string& fontname) const = 0;
};

class OptionsProvider {
  public:
    virtual bool getOptionBoolean(const std::string& optionName) const = 0;
    virtual std::string getOptionString(
        const std::string& optionName) const = 0;
    virtual double getOptionDouble(const std::string& optionName) const = 0;
    virtual Vec2 getOption2DVector(const std::string& optionName) const = 0;
    virtual std::vector<bool> getOptionBooleanArray(
        const std::string& optionName) const = 0;
    virtual std::vector<double> getOptionDoubleArray(
        const std::string& optionName) const = 0;
    virtual std::vector<std::string> getOptionStringArray(
        const std::string& optionName) const = 0;
    virtual std::vector<Vec2> getOption2DVectorArray(
        const std::string& optionName) const = 0;
    virtual bool isOptionSet(const std::string& optionName) const = 0;
    virtual void write(std::ostream& output) const = 0;

    virtual void setOptionBoolean(const std::string& optionName,
                                  bool optionValue) = 0;
    virtual void setOptionDouble(const std::string& optionName,
                                 double optionValue) = 0;
    virtual void setOptionString(const std::string& optionName,
                                 const std::string& optionValue) = 0;
    virtual void setOption2DVector(const std::string& optionName,
                                   Vec2 value) = 0;
    virtual void setOptionBooleanArray(
        const std::string& optionName,
        const std::vector<bool>& booleanArray) = 0;
    virtual void setOptionDoubleArray(
        const std::string& optionName,
        const std::vector<double>& doubleArray) = 0;
    virtual void setOptionStringArray(
        const std::string& optionName,
        const std::vector<std::string>& stringArray) = 0;
    virtual void setOption2DVectorArray(
        const std::string& optionName,
        const std::vector<Vec2>& vectorArray) = 0;
};

class TextureProvider {
  public:
    virtual const sf::Texture* getTexture(
        const std::string& texturename) const = 0;
};

class Services {
  public:
    Services();

    static const FontProvider* getFontService() {
        return getInstance()->m_fontProvider;
    }
    static const OptionsProvider* getOptionsService() {
        return getInstance()->m_optionsProvider;
    }
    static const TextureProvider* getTextureService() {
        return getInstance()->m_textureProvider;
    }

    void setFontProvider(const FontProvider* fontProvider) {
        m_fontProvider = fontProvider;
    }
    void setOptionsProvider(const OptionsProvider* optionsProvider) {
        m_optionsProvider = optionsProvider;
    }
    void setTextureProvider(const TextureProvider* textureProvider) {
        m_textureProvider = textureProvider;
    }

  private:
    static Services* getInstance();
    static Services* s_instance;

    const FontProvider* m_fontProvider;
    const OptionsProvider* m_optionsProvider;
    const TextureProvider* m_textureProvider;
};

}  // namespace VVipers

#endif  // VVIPERS_SERVICES_HPP
