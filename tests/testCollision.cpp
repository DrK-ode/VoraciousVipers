#include <gtest/gtest.h>

#include <vvipers/Collision/Collider.hpp>
#include <vvipers/Collision/ColliderManager.hpp>
#include <vvipers/Collision/Shape.hpp>
#include <vvipers/Utilities/debug.hpp>

using namespace VVipers;

namespace {

// TEST(CollisionTest, ShapesTest) {
//     RectangleShape rect1(Vec2(100, 100), true);
//     rect1.setPosition(0, 0);
//     RectangleShape rect2(Vec2(100, 100), false);
//     rect2.setPosition(0, 200);
//     EXPECT_EQ(Collider::collision(rect1, rect2).size(), 0);
//     rect2.setPosition(0, 50);
//     EXPECT_EQ(Collider::collision(rect1, rect2).size(), 1);
//     rect1.setActive(false);
//     EXPECT_EQ(Collider::collision(rect1, rect2).size(), 0);
//     CircleShape circle1(50, 10, true);
//     circle1.setPosition(100, 100);
//     EXPECT_EQ(Collider::collision(rect1, circle1).size(), 1);
//     circle1.setPosition(200, 200);
//     EXPECT_EQ(Collider::collision(rect1, circle1).size(), 0);
// }

TEST(CollisionTest, ManagerTest) {
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

    EXPECT_EQ(manager.checkForCollisions().size(), 2);
    rect2.setPosition(51, 51);
    EXPECT_EQ(manager.checkForCollisions().size(), 3);
}

}  // namespace