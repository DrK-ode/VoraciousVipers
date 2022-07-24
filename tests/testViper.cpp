#include <gtest/gtest.h>

#include <vvipers/Time.hpp>
#include <vvipers/Viper.hpp>
#include <vvipers/ViperConfig.hpp>
#include <vvipers/debug.hpp>

using namespace VVipers;

namespace {

class ViperTest : public ::testing::Test {
  protected:
    void SetUp() override {
        logLevel = LogLevel::onlyErrors;
        viper.setup(Vec2(0, 0), 180.f, 1.5);
    }

    Viper viper;
};

TEST_F(ViperTest, angleTest) {
    EXPECT_DOUBLE_EQ(viper.angle(), 180.);
    viper.angle(90.);
    EXPECT_DOUBLE_EQ(viper.angle(), 90.);
    viper.angle(-90.);
    EXPECT_DOUBLE_EQ(viper.angle(), -90.);
    viper.angle(270.);
    EXPECT_DOUBLE_EQ(viper.angle(), -90.);
}

TEST_F(ViperTest, lengthTest) {
    double expectedLength =
        toSeconds(ViperConfig::properties().headDuration +
                  1.5 * ViperConfig::properties().bodyDuration +
                  ViperConfig::properties().tailDuration) *
        viper.speed();
    viper.update(seconds(3.0));  // Let it grow
    EXPECT_DOUBLE_EQ(viper.length(), expectedLength);
    viper.update(seconds(3.0));  // Let it grow more
    EXPECT_DOUBLE_EQ(viper.length(), expectedLength);
}

}  // namespace