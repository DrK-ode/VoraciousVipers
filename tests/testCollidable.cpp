#include <gtest/gtest.h>

#include <vector>
#include <vvipers/Bodypart.hpp>
#include <vvipers/Collidable.hpp>
#include <vvipers/CollisionBody.hpp>
#include <vvipers/RectBody.hpp>
#include <vvipers/debug.hpp>

using namespace VVipers;

namespace {

class SimpleCollidable : public Collidable {
  public:
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
        RectBody* body;
        object1.addBody(
            body = new RectBody(Vec2(0, 0), Vec2(100, 100), "obj1part0", true));
        object1.addBody(body = new RectBody(Vec2(200, 0), Vec2(100, 100),
                                            "obj1part1", false));
        object2.addBody(body = new RectBody(Vec2(200, 0), Vec2(100, 100),
                                            "obj2part0", true));
        object2.addBody(body = new RectBody(Vec2(0, 0), Vec2(100, 100),
                                            "obj2part1", false));
        body->rectangleShape.setOrigin(50, 50);
        body->rectangleShape.setPosition(Vec2(150, 50));
        body->rectangleShape.rotate(-45);
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
    EXPECT_EQ(object1[0]->bodyParts()[0]->label(), "obj1part0");
    EXPECT_EQ(
        CollisionBody::collision(object1[0], object2[1]).front().first->label(),
        "obj1part0");
    EXPECT_EQ(CollisionBody::collision(object1[0], object2[1])
                  .front()
                  .second->label(),
              "obj2part1");
}

TEST_F(CollidableTest, CollidableTest) {
    EXPECT_EQ(Collidable::collision(&object1, &object2).size(), 2);
    EXPECT_EQ(Collidable::collision(&object1, &object2).front().first,
              CollisionTuple(&object1, object1[0], object1[0]->bodyParts()[0]));
    EXPECT_EQ(Collidable::collision(&object1, &object2).back().first,
              CollisionTuple(&object1, object1[1], object1[1]->bodyParts()[0]));
    EXPECT_EQ(Collidable::collision(&object2, &object2).front().first,
              CollisionTuple(&object2, object2[0], object2[0]->bodyParts()[0]));
    EXPECT_EQ(Collidable::collision(&object2, &object2).front().second,
              CollisionTuple(&object2, object2[1], object2[1]->bodyParts()[0]));
}

}  // namespace