#include <gtest/gtest.h>

#include <vvipers/Vec2.hpp>

using namespace VVipers;

namespace {

TEST(VectorMathTest, dotProductTest) {
    EXPECT_EQ(Vec2(1, 1).dot(Vec2(0, 0)), 0);
    EXPECT_EQ(Vec2(0, 1).dot(Vec2(1, 0)), 0);
    EXPECT_EQ(Vec2(1, 1).dot(Vec2(1, 1)), 2);
    EXPECT_EQ(Vec2(1, -1).dot(Vec2(1, 1)), 0);
}

TEST(VectorMathTest, absTest) {
    EXPECT_EQ(Vec2(1, 0).abs(), 1);
    EXPECT_EQ(Vec2(3, 4).abs(), 5);
    EXPECT_DOUBLE_EQ(Vec2(1, 1).abs(), 1.4142135623730951);
}

TEST(VectorMathTest, perpVecTest) {
    EXPECT_EQ(Vec2(1, 0).perpVec(), Vec2(0, 1));
    EXPECT_EQ(Vec2(0, 1).perpVec(), Vec2(-1, 0));
    EXPECT_EQ(Vec2(555, 666).perpVec().dot(Vec2(555, 666)), 0);
    EXPECT_EQ(Vec2(1, 1).perpVec().abs(), Vec2(1, 1).abs());
}

TEST(VectorMathTest, normalizeVecTest) {
    EXPECT_DOUBLE_EQ(Vec2(1, 1).normalize().abs(), 1.);
    EXPECT_EQ(Vec2(2, 0).normalize(), Vec2(1, 0.));
    EXPECT_DOUBLE_EQ(Vec2(1, 1).normalize(3).abs(), 3.);
    EXPECT_EQ(Vec2(2, 0).normalize(3), Vec2(3, 0.));
}

TEST(VectorMathTest, radToDegTest) {
    EXPECT_DOUBLE_EQ(0., radToDeg(0));
    EXPECT_DOUBLE_EQ(45., radToDeg(pi / 4));
    EXPECT_DOUBLE_EQ(90., radToDeg(pi / 2));
    EXPECT_DOUBLE_EQ(-45., radToDeg(-pi / 4));
    EXPECT_DOUBLE_EQ(-90., radToDeg(-pi / 2));
}

TEST(VectorMathTest, degToRadTest) {
    EXPECT_DOUBLE_EQ(0., degToRad(0));
    EXPECT_DOUBLE_EQ(pi / 4, degToRad(45));
    EXPECT_DOUBLE_EQ(pi / 2, degToRad(90));
    EXPECT_DOUBLE_EQ(-pi / 4, degToRad(-45));
    EXPECT_DOUBLE_EQ(-pi / 2, degToRad(-90));
}

TEST(VectorMathTest, modDegTest) {
    EXPECT_DOUBLE_EQ(90., mod180Deg(90));
    EXPECT_DOUBLE_EQ(-90., mod180Deg(-90));
    EXPECT_DOUBLE_EQ(0., mod180Deg(360));
    EXPECT_DOUBLE_EQ(-90., mod180Deg(270));
    EXPECT_DOUBLE_EQ(0., mod180Deg(-360));
    EXPECT_DOUBLE_EQ(90., mod180Deg(-270));
}

TEST(VectorMathTest, modRadTest) {
    EXPECT_DOUBLE_EQ(pi / 2, modPiRad(pi / 2));
    EXPECT_DOUBLE_EQ(-pi / 2, modPiRad(-pi / 2));
    EXPECT_DOUBLE_EQ(0, modPiRad(twopi));
    EXPECT_DOUBLE_EQ(-pi / 2, modPiRad(3 * pi / 2));
    EXPECT_DOUBLE_EQ(0, modPiRad(-twopi));
    EXPECT_DOUBLE_EQ(pi / 2, modPiRad(-3 * pi / 2));
}

}  // namespace