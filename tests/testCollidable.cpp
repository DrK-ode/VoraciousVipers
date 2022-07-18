#include <gtest/gtest.h>

#include <vvipers/CollisionBody.hpp>
#include <vvipers/debug.hpp>

using namespace VVipers;

namespace {

// class CollidableObject1 : public Collidable {
//   public:
//     CollidableObject1() {
//         m_collidableParts.push_back( new RectCollidable( Vec2(0,1), Vec2(1,1) ) );
//         auto cp2 = new CollidableNodes;
//         cp2->resize(3);
//         cp2->node(0, Vec2(1, 0));
//         cp2->node(1, Vec2(1, 1));
//         cp2->node(2, Vec2(2, 0.5));
//         cp2->update();
//         m_collidableParts.push_back(cp2);
//     }
//     std::vector<ConvexCollidable*>& getParts() {return m_collidableParts;}
//   private:
//     void updateNodes() override {}
// };

// class CollidableObject2 : public Collidable {
//   public:
//     CollidableObject2(){
//         m_collidableParts.push_back( new RectCollidable( Vec2(-2,1), Vec2(1,1) ) );
//         auto cp2 = new CollidableNodes;
//         cp2->resize(3);
//         cp2->node(0, Vec2(-1, 0));
//         cp2->node(1, Vec2(-1, 1));
//         cp2->node(2, Vec2(0.5, 0.5));
//         cp2->update();
//         m_collidableParts.push_back(cp2);
//     }
//     std::vector<ConvexCollidable*>& getParts() {return m_collidableParts;}
//   private:
//     void updateNodes() override {}
// };

// class CollidableTest : public ::testing::Test {
//   protected:
//     void SetUp() override {}
//     CollidableObject1 obj1;
//     CollidableObject2 obj2;
// };

// class CollidablePartTest : public ::testing::Test {
//   protected:
//     void SetUp() override {
//       cp1.resize(4);
//         cp1.node(0, Vec2(0, 0));
//         cp1.node(1, Vec2(0, 1));
//         cp1.node(2, Vec2(1, 1));
//         cp1.node(3, Vec2(1, 0));
//         cp1.update();
//         cp2.resize(4);
//         cp2.node(0, Vec2(2, 0));
//         cp2.node(1, Vec2(2, 1));
//         cp2.node(2, Vec2(3, 1));
//         cp2.node(3, Vec2(3, 0));
//         cp2.update();
//         cp3.resize(4);
//         cp3.node(0, Vec2(0, 0) + Vec2(0.5, 0.5));
//         cp3.node(1, Vec2(0, 1) + Vec2(0.5, 0.5));
//         cp3.node(2, Vec2(1, 1) + Vec2(0.5, 0.5));
//         cp3.node(3, Vec2(1, 0) + Vec2(0.5, 0.5));
//         cp3.update();
//         cp4.resize(3);
//         cp4.node(0, Vec2(0.6, 0.9));
//         cp4.node(1, Vec2(0, 2));
//         cp4.node(2, Vec2(1, 2));
//         cp4.update();
//         cp5.resize(3);
//         cp5.node(0, Vec2(0.6, 0.9));
//         cp5.node(1, Vec2(0, -2));
//         cp5.node(2, Vec2(1, -2));
//         cp5.update();
//     }

//     CollidableNodes cp1;
//     CollidableNodes cp2;
//     CollidableNodes cp3;
//     CollidableNodes cp4;
//     CollidableNodes cp5;
// };

// TEST_F(CollidablePartTest, rectPartsTest) {
//     EXPECT_FALSE(ConvexCollidable::collision(&cp1, &cp2));
//     EXPECT_TRUE(ConvexCollidable::collision(&cp1, &cp3));
//     EXPECT_FALSE(ConvexCollidable::collision(&cp2, &cp3));
// }

// TEST_F(CollidablePartTest, trianglePartsTest) {
//     EXPECT_TRUE(ConvexCollidable::collision(&cp1, &cp4));
//     EXPECT_FALSE(ConvexCollidable::collision(&cp2, &cp4));
//     EXPECT_TRUE(ConvexCollidable::collision(&cp3, &cp4));
//     EXPECT_TRUE(ConvexCollidable::collision(&cp5, &cp4));
// }

// TEST_F(CollidableTest, collisionTest) {
//     EXPECT_FALSE(Collidable::collision(obj1, obj2).size() > 0);
//     obj1.getParts()[1]->active(true);
//     EXPECT_FALSE(Collidable::collision(obj1, obj2).size() > 0);
//     obj1.getParts()[1]->active(false);
//     obj1.getParts()[0]->active(true);
//     EXPECT_TRUE(Collidable::collision(obj1, obj2).size() > 0);
//     obj1.getParts()[0]->active(false);
//     obj2.getParts()[0]->active(true);
//     EXPECT_FALSE(Collidable::collision(obj1, obj2).size() > 0);
//     obj2.getParts()[0]->active(false);
//     obj2.getParts()[1]->active(true);
//     EXPECT_TRUE(Collidable::collision(obj1, obj2).size() > 0);
// }

}  // namespace