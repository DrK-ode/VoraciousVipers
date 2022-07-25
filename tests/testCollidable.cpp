#include <gtest/gtest.h>

#include <vector>
#include <vvipers/Bodypart.hpp>
#include <vvipers/Collidable.hpp>
#include <vvipers/CollisionBody.hpp>
#include <vvipers/ConvexBody.hpp>
#include <vvipers/debug.hpp>

using namespace VVipers;

namespace {

class SimpleCollidable : public Collidable {
  public:
    SimpleCollidable() {}
    const CollisionBody* operator[](int i) const { return m_bodies[i]; }
    void addBody(CollisionBody* b) { m_bodies.push_back(b); }
    std::vector<const CollisionBody*> collisionBodies() const override {
        return m_bodies;
    }
    std::vector<const CollisionBody*> m_bodies;
};

class CollidableTest : public ::testing::Test {
  protected:
    void SetUp() override {
        ConvexBody* body;
        object1.addBody(body = ConvexBody::createRectangle(
                            Vec2(0, 0), Vec2(100, 100), true));
        object1.addBody(body = ConvexBody::createRectangle(
                            Vec2(200, 0), Vec2(100, 100), false));
        object2.addBody(body = ConvexBody::createRectangle(
                            Vec2(200, 0), Vec2(100, 100), true));
        object2.addBody(body = ConvexBody::createRectangle(
                            Vec2(0, 0), Vec2(100, 100), false));
        body->convexShape.setOrigin(50, 50);
        body->convexShape.setPosition(Vec2(150, 50));
        body->convexShape.rotate(-45);
        body->updateBodyPart();
    }
    SimpleCollidable object1;
    SimpleCollidable object2;
};

TEST_F(CollidableTest, RectTest) {
    EXPECT_EQ(CollisionBody::collision(object1[0], object2[0]).size(), 0);
    EXPECT_EQ(CollisionBody::collision(object1[1], object2[0]).size(), 1);
    EXPECT_EQ(CollisionBody::collision(object1[0], object2[1]).size(), 1);
    EXPECT_EQ(CollisionBody::collision(object1[1], object2[1]).size(), 0);
}

TEST_F(CollidableTest, CollidableTest) {
    EXPECT_EQ(Collidable::collision(&object1, &object2).size(), 2);
    EXPECT_EQ(
        Collidable::collision(&object1, &object2).front().first,
        CollisionTriplet(&object1, object1[0], object1[0]->bodyparts()[0]));
    EXPECT_EQ(
        Collidable::collision(&object1, &object2).back().first,
        CollisionTriplet(&object1, object1[1], object1[1]->bodyparts()[0]));
    EXPECT_EQ(
        Collidable::collision(&object2, &object2).front().first,
        CollisionTriplet(&object2, object2[0], object2[0]->bodyparts()[0]));
    EXPECT_EQ(
        Collidable::collision(&object2, &object2).front().second,
        CollisionTriplet(&object2, object2[1], object2[1]->bodyparts()[0]));
}

}  // namespace