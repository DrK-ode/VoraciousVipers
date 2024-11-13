#include <gtest/gtest.h>

#include <vvipers/Collisions/CollidingBody.hpp>
#include <vvipers/Collisions/CollidingSegment.hpp>
#include <vvipers/Collisions/CollisionManager.hpp>
#include <vvipers/Utilities/debug.hpp>

using namespace VVipers;

namespace {

/*TEST(CollisionTest, ManagerTest) {
    RectangleShape rect1(Vec2(100, 100), true);
    RectangleShape rect2(Vec2(100, 100), true);
    CircleShape circle1(50, 10, true);
    ColliderManager manager;
    manager.registerCollider(rect1);
    manager.registerCollider(rect2);
    manager.registerCollider(circle1);

    rect1.setPosition(0, 0);
    rect2.setPosition(100, 100);
    circle1.setPosition(100, 100);

    EXPECT_EQ(manager.check_for_collisions().size(), 2);
    rect2.setPosition(51, 51);
    EXPECT_EQ(manager.check_for_collisions().size(), 3);
    }*/

}  // namespace