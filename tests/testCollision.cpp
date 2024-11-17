#include <gtest/gtest.h>

#include <memory>
#include <vvipers/Collisions/CollidingBody.hpp>
#include <vvipers/Collisions/CollisionManager.hpp>
#include <vvipers/Utilities/Shape.hpp>
#include <vvipers/Utilities/debug.hpp>

using namespace VVipers;

namespace {

class Body : public CollidingBody {
  public:
    Body(std::shared_ptr<const Shape> shape) : CollidingBody("TestBody"), _shape(shape) {}
    virtual size_t number_of_segments() const override { return 1; }
    virtual std::shared_ptr<const Shape> segment_shape(size_t index) const override { return _shape; }
    const std::shared_ptr<const Shape> _shape;
};

TEST(CollisionTest, ShapeTest) {
    std::vector<Vec2> corners;
    corners.emplace_back(0, 0);
    corners.emplace_back(100, 0);
    corners.emplace_back(100, 100);
    corners.emplace_back(0, 100);
    Polygon poly(corners);
    Circle circle1(Vec2(100, 100), 50);
    Circle circle2(Vec2(200, 200), 50);
    EXPECT_TRUE(poly.overlap(circle1));
    EXPECT_FALSE(poly.overlap(circle2));
}

TEST(CollisionTest, ManagerTest) {
    std::vector<Vec2> corners;
    corners.emplace_back(0, 0);
    corners.emplace_back(100, 0);
    corners.emplace_back(100, 100);
    corners.emplace_back(0, 100);
    Body body1(std::make_shared<Polygon>(corners));
    corners.clear();
    corners.emplace_back(100, 100);
    corners.emplace_back(200, 100);
    corners.emplace_back(200, 200);
    corners.emplace_back(100, 200);
    Body body2(std::make_shared<Polygon>(corners));
    Body body3(std::make_shared<Circle>(Vec2(100, 100), 50));
    corners.clear();
    corners.emplace_back(25, 25);
    corners.emplace_back(25, 50);
    corners.emplace_back(50, 50);
    corners.emplace_back(50, 25);
    Body body4(std::make_shared<Polygon>(corners));

    CollisionManager manager(4, 500);
    manager.register_colliding_body(&body1);
    manager.register_colliding_body(&body2);
    manager.register_colliding_body(&body3);
    EXPECT_EQ(manager.check_for_collisions(BoundingBox(0, 500, 0, 500)).size(), 2);
    manager.register_colliding_body(&body4);
    EXPECT_EQ(manager.check_for_collisions(BoundingBox(0, 500, 0, 500)).size(), 3);
}

}  // namespace