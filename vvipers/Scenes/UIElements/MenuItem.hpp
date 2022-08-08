#ifndef VVIPERS_SCENES_UIELEMENTS_MENUITEM_HPP
#define VVIPERS_SCENES_UIELEMENTS_MENUITEM_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Utilities/Time.hpp>

namespace VVipers {

class MenuItem : public sf::Drawable {
  public:
    MenuItem() : m_selected(false){};
    bool contains(double x, double y) const {
        return getLocalBounds().contains(x, y);
    }
    bool contains(Vec2 point) const { return contains(point.x, point.y); }
    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates states) const override{};
    virtual void setColors( sf::Color fill, sf::Color border, sf::Color text) {};
    virtual void update(Time elapsedTime){};
    void setSelected(bool selected);
    bool isSelected() const { return m_selected; }
    sf::FloatRect getLocalBounds() const;
    Vec2 getSize() const { return m_size; }
    void setSize(Vec2 size);
    Vec2 getPosition() const { return m_position; }
    void setPosition(Vec2 position);
    virtual void onGeometryChange(){};
    virtual void onSelection(){};
    virtual void handleEvent(const sf::Event& event){};

  private:
    Vec2 m_size;
    Vec2 m_position;
    bool m_selected;
};

}  // namespace VVipers

#endif  // VVIPERS_SCENES_UIELEMENTS_MENUITEM_HPP
