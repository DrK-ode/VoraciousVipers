#include <vvipers/CollisionBody.hpp>
#include <vvipers/Bodypart.hpp>

namespace VVipers {

std::vector<std::pair<const Bodypart*, const Bodypart*> >
CollisionBody::collision(const CollisionBody* body1,
                         const CollisionBody* body2) {
    std::vector<std::pair<const Bodypart*, const Bodypart*> > collidingParts;
    for (const auto part1 : body1->bodyParts()) {
        for (const auto part2 : body2->bodyParts()) {
            if (part1->active() || part2->active()) {
                if (Bodypart::collision(part1, part2))
                    collidingParts.push_back(std::pair(part1, part2));
            }
        }
    }
    return collidingParts;
}

}  // namespace VVipers