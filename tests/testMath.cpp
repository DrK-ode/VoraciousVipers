#include <gtest/gtest.h>

#include <vvipers/Utilities/VVMath.hpp>
#include <vvipers/Utilities/Vec2.hpp>

using namespace VVipers;

namespace {

TEST(VectorMathTest, dotProductTest) {
    EXPECT_EQ(Vec2(1, 1).dot(Vec2(0, 0)), 0);
    EXPECT_EQ(Vec2(0, 1).dot(Vec2(1, 0)), 0);
    EXPECT_EQ(Vec2(1, 1).dot(Vec2(1, 1)), 2);
    EXPECT_EQ(Vec2(1, -1).dot(Vec2(1, 1)), 0);
    EXPECT_EQ(Vec2(1, 1).dot(Vec2(-1, -1)), -2);
}

TEST(VectorMathTest, absTest) {
    EXPECT_EQ(Vec2(1, 0).abs(), 1);
    EXPECT_EQ(Vec2(3, 4).abs(), 5);
    EXPECT_DOUBLE_EQ(Vec2(1, 1).abs(), 1.4142135623730951);
}

TEST(VectorMathTest, perpVecTest) {
    EXPECT_EQ(Vec2(1, 0).perpendicular(), Vec2(0, 1));
    EXPECT_EQ(Vec2(0, 1).perpendicular(), Vec2(-1, 0));
    EXPECT_EQ(Vec2(555, 666).perpendicular().dot(Vec2(555, 666)), 0);
    EXPECT_EQ(Vec2(1, 1).perpendicular().abs(), Vec2(1, 1).abs());
}

TEST(VectorMathTest, normalizeVecTest) {
    EXPECT_DOUBLE_EQ(Vec2(1, 1).normalized().abs(), 1.);
    EXPECT_EQ(Vec2(2, 0).normalized(), Vec2(1, 0.));
    EXPECT_DOUBLE_EQ(Vec2(1, 1).normalized(3).abs(), 3.);
    EXPECT_EQ(Vec2(2, 0).normalized(3), Vec2(3, 0.));
    EXPECT_EQ(Vec2(1, 0).normalized(-1), Vec2(-1, 0));
}

TEST(VectorMathTest, projectionVecTest) {
    EXPECT_EQ(Vec2(1, 1).vector_projection(Vec2(1, 0)), Vec2(1, 0));
    EXPECT_EQ(Vec2(1, 1).vector_projection(Vec2(0, 0)), Vec2(0, 0));
    EXPECT_EQ(Vec2(1, 1).vector_projection(Vec2(-1, 0)), Vec2(1, 0));
    EXPECT_EQ(Vec2(1, 1).vector_projection(Vec2(0, -1)), Vec2(0, 1));
    EXPECT_DOUBLE_EQ(Vec2(1, 1).scalar_projection(Vec2(1, 0)), 1.);
    EXPECT_DOUBLE_EQ(Vec2(1, 1).scalar_projection(Vec2(0, 0)), 0.);
    EXPECT_DOUBLE_EQ(Vec2(1, 1).scalar_projection(Vec2(-1, 0)), -1.);
    EXPECT_DOUBLE_EQ(Vec2(1, 1).scalar_projection(Vec2(0, -1)), -1.);
    // Rounding errors unless you compare x and y separately
    EXPECT_DOUBLE_EQ(Vec2(1, 1).vector_projection(Vec2(-1, -1)).x, 1.);
    EXPECT_DOUBLE_EQ(Vec2(1, 1).vector_projection(Vec2(-1, -1)).y, 1.);
}

TEST(VectorMathTest, rotationTest) {
    EXPECT_DOUBLE_EQ(Vec2(1, 0).rotate(pi / 2.).y, 1.);
    EXPECT_DOUBLE_EQ(Vec2(1, 0).rotate(-pi / 2.).y, -1.);
    EXPECT_DOUBLE_EQ(Vec2(0, 1).rotate(pi / 2.).x, -1.);
    EXPECT_DOUBLE_EQ(Vec2(0, 1).rotate(-pi / 2.).x, 1.);
}

TEST(VectorMathTest, radToDegTest) {
    EXPECT_DOUBLE_EQ(0., deg_from_rad(0));
    EXPECT_DOUBLE_EQ(45., deg_from_rad(pi / 4));
    EXPECT_DOUBLE_EQ(90., deg_from_rad(pi / 2));
    EXPECT_DOUBLE_EQ(-45., deg_from_rad(-pi / 4));
    EXPECT_DOUBLE_EQ(-90., deg_from_rad(-pi / 2));
}

TEST(VectorMathTest, degToRadTest) {
    EXPECT_DOUBLE_EQ(0., rad_from_deg(0));
    EXPECT_DOUBLE_EQ(pi / 4, rad_from_deg(45));
    EXPECT_DOUBLE_EQ(pi / 2, rad_from_deg(90));
    EXPECT_DOUBLE_EQ(-pi / 4, rad_from_deg(-45));
    EXPECT_DOUBLE_EQ(-pi / 2, rad_from_deg(-90));
}

TEST(VectorMathTest, modDegTest) {
    EXPECT_DOUBLE_EQ(90., mod_180_deg(90));
    EXPECT_DOUBLE_EQ(-90., mod_180_deg(-90));
    EXPECT_DOUBLE_EQ(0., mod_180_deg(360));
    EXPECT_DOUBLE_EQ(-90., mod_180_deg(270));
    EXPECT_DOUBLE_EQ(0., mod_180_deg(-360));
    EXPECT_DOUBLE_EQ(90., mod_180_deg(-270));
}

TEST(VectorMathTest, modRadTest) {
    EXPECT_DOUBLE_EQ(pi / 2, mod_pi_rad(pi / 2));
    EXPECT_DOUBLE_EQ(-pi / 2, mod_pi_rad(-pi / 2));
    EXPECT_DOUBLE_EQ(0, mod_pi_rad(twopi));
    EXPECT_DOUBLE_EQ(-pi / 2, mod_pi_rad(3 * pi / 2));
    EXPECT_DOUBLE_EQ(0, mod_pi_rad(-twopi));
    EXPECT_DOUBLE_EQ(pi / 2, mod_pi_rad(-3 * pi / 2));
}

}  // namespace