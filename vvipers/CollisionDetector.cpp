#include <vvipers/CollisionDetector.hpp>
#include <vvipers/GameEvent.hpp>

namespace VVipers{
    
void CollisionDetector::checkForCollisions() const {
    auto c1 = m_collidables.cbegin();
    while (c1 != m_collidables.cend() ){
        auto c2 = c1;
        ++c2;
        while( c2 != m_collidables.cend() ){
            const Collidable* A = *c1;
            const Collidable* B = *c2;
            const auto& partCollisions = Collidable::collision(*A, *B);
            for ( auto& partCollision : partCollisions ){
                CollisionEvent event( A, partCollision.first, B, partCollision.second );
                notify( &event );
            }
            ++c2;
        }
        ++c1;
    }
}

}