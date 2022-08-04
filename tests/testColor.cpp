#include <gtest/gtest.h>

#include <vvipers/Utilities/VVColor.hpp>

using namespace VVipers;

namespace {

TEST(ColorTest, stringToColorTest) {
    EXPECT_EQ(colorFromRGBString("#87fb1a"), sf::Color(0x87, 0xfb, 0x1a));
}

TEST(ColorTest, HSLToColorTest) {
    EXPECT_EQ(colorFromHSL(0., 0., 0.), sf::Color::Black);
    EXPECT_EQ(colorFromHSL(0., 0., 1.), sf::Color::White);
    EXPECT_EQ(colorFromHSL(0., 1., 0.5), sf::Color::Red);
    EXPECT_EQ(colorFromHSL(1. / 3., 1., 0.5), sf::Color::Green);
    EXPECT_EQ(colorFromHSL(2. / 3., 1., 0.5), sf::Color::Blue);
}

TEST(ColorTest, RGBToHSLTest) {
    EXPECT_EQ(colorFromHSL(0, 1, 0.5), colorFromRGB(1, 0, 0));
    EXPECT_EQ(colorFromRGB(0.562, 0.375, 0.188), sf::Color(0x8f, 0x60, 0x30));
    EXPECT_NEAR(0x8f, colorFromHSL(1. / 12., 0.5, 3. / 8.).r, 1);
    EXPECT_NEAR(0x60, colorFromHSL(1. / 12., 0.5, 3. / 8.).g, 1);
    EXPECT_NEAR(0x30, colorFromHSL(1. / 12., 0.5, 3. / 8.).b, 1);
}

}  // namespace