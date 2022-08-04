#include <gtest/gtest.h>

#include <vvipers/Utilities/VVColor.hpp>

using namespace VVipers;

namespace {

TEST(ColorTest, stringToColorTest) {
    EXPECT_EQ( colorFromRGBString("#87fb1a"), sf::Color(0x87,0xfb,0x1a));
}

TEST(ColorTest, HSLToColorTest) {
    EXPECT_EQ( colorFromHSL(0.,0.,0.), sf::Color::Black);
    EXPECT_EQ( colorFromHSL(0.,0.,1.), sf::Color::White);
    EXPECT_EQ( colorFromHSL(0.,1.,0.5), sf::Color::Red);
    EXPECT_EQ( colorFromHSL(1./3.,1.,0.5), sf::Color::Green);
    EXPECT_EQ( colorFromHSL(2./3.,1.,0.5), sf::Color::Blue);
}

}  // namespace