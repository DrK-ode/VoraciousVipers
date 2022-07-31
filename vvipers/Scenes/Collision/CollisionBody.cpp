#include <vvipers/Scenes/Collision/Bodypart.hpp>
#include <vvipers/Scenes/Collision/CollisionBody.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

std::vector<std::pair<const Bodypart*, const Bodypart*> >
CollisionBody::collision(const CollisionBody* body1,
                         const CollisionBody* body2) {
    std::vector<std::pair<const Bodypart*, const Bodypart*> > collidingParts;
    // Rough check before doing it properly
    if (body1 == body2 or
        !body1->rectangularBounds().intersects(body2->rectangularBounds()))
        return collidingParts; // No collision

    for (const auto part1 : body1->bodyparts()) {
        for (const auto part2 : body2->bodyparts()) {
            if (part1->active() || part2->active()) {
                if (Bodypart::collision(part1, part2))
                    collidingParts.push_back(std::pair(part1, part2));
            }
        }
    }
    return collidingParts;
}

sf::Rect<double> CollisionBody::rectangularBounds() const {
    double xmin = std::numeric_limits<double>::max();
    double ymin = std::numeric_limits<double>::max();
    double xmax = std::numeric_limits<double>::lowest();
    double ymax = std::numeric_limits<double>::lowest();

    for (auto bodypart : bodyparts()) {
        auto bounds = bodypart->rectangularBounds();
        xmin = std::min(xmin, bounds.left);
        xmax = std::max(xmax, bounds.left + bounds.width);
        ymin = std::min(ymin, bounds.top);
        ymax = std::max(ymax, bounds.top + bounds.height);
    }
    return sf::Rect<double>(xmin, ymin, xmax - xmin, ymax - ymin);
}

}  // namespace VVipers