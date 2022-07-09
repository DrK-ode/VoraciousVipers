#include <gtest/gtest.h>

#include <vvipers/Collidable.hpp>
#include <vvipers/debug.hpp>

using namespace VVipers;

namespace {

class CollidableObject1 : public Collidable {
  public:
    CollidableObject1() {
        CollidablePart cp1;
        cp1.nodes.push_back(Vec2(0, 0));
        cp1.nodes.push_back(Vec2(0, 1));
        cp1.nodes.push_back(Vec2(1, 1));
        cp1.nodes.push_back(Vec2(1, 0));
        cp1.updateEdgesAndNormals();
        m_collidableParts.push_back(cp1);
        CollidablePart cp2;
        cp2.nodes.push_back(Vec2(1, 0));
        cp2.nodes.push_back(Vec2(1, 1));
        cp2.nodes.push_back(Vec2(2, 0.5));
        cp2.updateEdgesAndNormals();
        m_collidableParts.push_back(cp2);
    }
    std::vector<CollidablePart>& getParts() {return m_collidableParts;}
  private:
    void updateNodes() override {}
};

class CollidableObject2 : public Collidable {
  public:
    CollidableObject2(){
        CollidablePart cp1;
        cp1.nodes.push_back(Vec2(-1, 0));
        cp1.nodes.push_back(Vec2(-2, 0));
        cp1.nodes.push_back(Vec2(-2, 1));
        cp1.nodes.push_back(Vec2(-1, 0));
        cp1.updateEdgesAndNormals();
        m_collidableParts.push_back(cp1);
        CollidablePart cp2;
        cp2.nodes.push_back(Vec2(-1, 0));
        cp2.nodes.push_back(Vec2(-1, 1));
        cp2.nodes.push_back(Vec2(0.5, 0.5));
        cp2.updateEdgesAndNormals();
        m_collidableParts.push_back(cp2);
    }
    std::vector<CollidablePart>& getParts() {return m_collidableParts;}
  private:
    void updateNodes() override {}
};

class CollidableTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    CollidableObject1 obj1;
    CollidableObject2 obj2;
};

class CollidablePartTest : public ::testing::Test {
  protected:
    void SetUp() override {
        cp1.nodes.push_back(Vec2(0, 0));
        cp1.nodes.push_back(Vec2(0, 1));
        cp1.nodes.push_back(Vec2(1, 1));
        cp1.nodes.push_back(Vec2(1, 0));
        cp1.updateEdgesAndNormals();
        cp2.nodes.push_back(Vec2(2, 0));
        cp2.nodes.push_back(Vec2(2, 1));
        cp2.nodes.push_back(Vec2(3, 1));
        cp2.nodes.push_back(Vec2(3, 0));
        cp2.updateEdgesAndNormals();
        cp3.nodes.push_back(Vec2(0, 0) + Vec2(0.5, 0.5));
        cp3.nodes.push_back(Vec2(0, 1) + Vec2(0.5, 0.5));
        cp3.nodes.push_back(Vec2(1, 1) + Vec2(0.5, 0.5));
        cp3.nodes.push_back(Vec2(1, 0) + Vec2(0.5, 0.5));
        cp3.updateEdgesAndNormals();
        cp4.nodes.push_back(Vec2(0.6, 0.9));
        cp4.nodes.push_back(Vec2(0, 2));
        cp4.nodes.push_back(Vec2(1, 2));
        cp4.updateEdgesAndNormals();
        cp5.nodes.push_back(Vec2(0.6, 0.9));
        cp5.nodes.push_back(Vec2(0, -2));
        cp5.nodes.push_back(Vec2(1, -2));
        cp5.updateEdgesAndNormals();
    }

    CollidablePart cp1;
    CollidablePart cp2;
    CollidablePart cp3;
    CollidablePart cp4;
    CollidablePart cp5;
};

TEST_F(CollidablePartTest, rectPartsTest) {
    EXPECT_FALSE(CollidablePart::collision(cp1, cp2));
    EXPECT_TRUE(CollidablePart::collision(cp1, cp3));
    EXPECT_FALSE(CollidablePart::collision(cp2, cp3));
}

TEST_F(CollidablePartTest, trianglePartsTest) {
    EXPECT_TRUE(CollidablePart::collision(cp1, cp4));
    EXPECT_FALSE(CollidablePart::collision(cp2, cp4));
    EXPECT_TRUE(CollidablePart::collision(cp3, cp4));
    EXPECT_TRUE(CollidablePart::collision(cp5, cp4));
}

TEST_F(CollidableTest, collisionTest) {
    EXPECT_FALSE(Collidable::collision(obj1, obj2));
    obj1.getParts()[1].isActive = true;
    EXPECT_FALSE(Collidable::collision(obj1, obj2));
    obj1.getParts()[1].isActive = false;
    obj1.getParts()[0].isActive = true;
    EXPECT_TRUE(Collidable::collision(obj1, obj2));
    obj1.getParts()[0].isActive = false;
    obj2.getParts()[0].isActive = true;
    EXPECT_FALSE(Collidable::collision(obj1, obj2));
    obj2.getParts()[0].isActive = false;
    obj2.getParts()[1].isActive = true;
    EXPECT_TRUE(Collidable::collision(obj1, obj2));
}

}  // namespace