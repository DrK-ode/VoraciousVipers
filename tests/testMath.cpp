#include <gtest/gtest.h>

#include <vvipers/VectorMath.hpp>

using namespace VVipers;

namespace {

TEST(VectorMathTest, typeDefTest) {
    EXPECT_EQ(Vec2i(), Vec2(0, 0));
    EXPECT_EQ(Vec2u(), Vec2(0u, 0u));
    EXPECT_EQ(Vec2f(), Vec2(0.f, 0.f));
}

TEST(VectorMathTest, dotProductTest) {
    EXPECT_EQ(Vec2(1, 1).dot(Vec2(0, 0)), 0);
    EXPECT_EQ(Vec2(0, 1).dot(Vec2(1, 0)), 0);
    EXPECT_EQ(Vec2(1, 1).dot(Vec2(1, 1)), 2);
    EXPECT_EQ(Vec2(1, -1).dot(Vec2(1, 1)), 0);
}

TEST(VectorMathTest, absTest) {
    EXPECT_EQ(Vec2(1, 0).abs(), 1);
    EXPECT_EQ(Vec2(3, 4).abs(), 5);
    EXPECT_FLOAT_EQ(Vec2f(1, 1).abs(), 1.414213562);
}

TEST(VectorMathTest, perpVecTest) {
    EXPECT_EQ(Vec2(1, 0).perpVec(), Vec2(0, 1));
    EXPECT_EQ(Vec2(0, 1).perpVec(), Vec2(-1, 0));
    EXPECT_EQ(Vec2(555, 666).perpVec().dot(Vec2(555, 666)), 0);
    EXPECT_EQ(Vec2(1, 1).perpVec().abs(), Vec2(1, 1).abs());
}

// Only relevant to test float variant Vec2<int>::normalize() should not be used
TEST(VectorMathTest, normalizeVecTest) {
    EXPECT_FLOAT_EQ(Vec2f(1, 1).normalize().abs(), 1.f);
    EXPECT_EQ(Vec2f(2, 0).normalize(), Vec2f(1, 0.f));
    EXPECT_FLOAT_EQ(Vec2f(1, 1).normalize(3).abs(), 3.f);
    EXPECT_EQ(Vec2f(2, 0).normalize(3), Vec2f(3, 0.f));
}

TEST(VectorMathTest, radToDegTest) {
    EXPECT_FLOAT_EQ(0.f, radToDeg(0));
    EXPECT_FLOAT_EQ(45.f, radToDeg(pi / 4));
    EXPECT_FLOAT_EQ(90.f, radToDeg(pi / 2));
    EXPECT_FLOAT_EQ(-45.f, radToDeg(-pi / 4));
    EXPECT_FLOAT_EQ(-90.f, radToDeg(-pi / 2));
}

TEST(VectorMathTest, degToRadTest) {
    EXPECT_FLOAT_EQ(0.f, degToRad(0));
    EXPECT_FLOAT_EQ(pi / 4, degToRad(45));
    EXPECT_FLOAT_EQ(pi / 2, degToRad(90));
    EXPECT_FLOAT_EQ(-pi / 4, degToRad(-45));
    EXPECT_FLOAT_EQ(-pi / 2, degToRad(-90));
}

TEST(VectorMathTest, modDegTest) {
    EXPECT_FLOAT_EQ(90.f, mod180Deg(90));
    EXPECT_FLOAT_EQ(-90.f, mod180Deg(-90));
    EXPECT_FLOAT_EQ(0.f, mod180Deg(360));
    EXPECT_FLOAT_EQ(-90.f, mod180Deg(270));
    EXPECT_FLOAT_EQ(0.f, mod180Deg(-360));
    EXPECT_FLOAT_EQ(90.f, mod180Deg(-270));
}

TEST(VectorMathTest, modRadTest) {
    EXPECT_FLOAT_EQ(pi / 2.f, modPiRad(pi / 2));
    EXPECT_FLOAT_EQ(-pi / 2.f, modPiRad(-pi / 2));
    EXPECT_FLOAT_EQ(0.f, modPiRad(twopi));
    EXPECT_FLOAT_EQ(-pi / 2.f, modPiRad(3 * pi / 2));
    EXPECT_FLOAT_EQ(0.f, modPiRad(-twopi));
    EXPECT_FLOAT_EQ(pi / 2.f, modPiRad(-3 * pi / 2));
}

}  // namespace