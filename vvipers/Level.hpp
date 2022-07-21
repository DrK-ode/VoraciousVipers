#ifndef VVIPERS_LEVEL_HPP
#define VVIPERS_LEVEL_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <string>
#include <vector>
#include <vvipers/Collidable.hpp>
#include <vvipers/CollisionBody.hpp>

namespace VVipers {

class RectBody;

class Level : public sf::Drawable, public Collidable {
  public:
    Level(CID_type id, const std::string& name);
    ~Level();

    enum class LevelObject : CBID_type {Wall};

    std::vector<const CollisionBody*> collisionBodies() const override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  protected:
    virtual void constructLevel();

  private:
    std::string m_name;
    std::vector<const RectBody*> m_rects;
};

}  // namespace VVipers

#endif