#ifndef VVIPERS_GAMEELEMENTS_WALLS_HPP
#define VVIPERS_GAMEELEMENTS_WALLS_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <vector>
#include <vvipers/Collision/Collider.hpp>
#include <vvipers/Collision/Shape.hpp>
#include <vvipers/GameElements/GameObject.hpp>

namespace VVipers {

class Walls : public GameObject, public sf::Drawable, public ColliderSegmented {
  public:
    Walls(Vec2 levelSize);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    size_t getSegmentCount() const override { return m_rects.size(); }
    size_t getSegmentPointCount(size_t) const override { return 4; }
    Vec2 getSegmentGlobalPoint(size_t i, size_t j) const override {
        return m_rects[i].getGlobalPoint(j);
    };
    bool isSegmentActive(size_t i) const override { return false; }

  protected:
    virtual void constructLevel();

  private:
    Vec2 m_levelSize;
    std::vector<RectangleShape> m_rects;
};

}  // namespace VVipers

#endif  // VVIPERS_GAMEELEMENTS_WALLS_HPP
