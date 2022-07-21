#include <gtest/gtest.h>

#include <vvipers/Time.hpp>
#include <vvipers/Viper.hpp>
#include <vvipers/debug.hpp>

using namespace VVipers;

namespace {

class ViperTest : public ::testing::Test {
  protected:
    void SetUp() override {
        logLevel = LogLevel::onlyErrors;
        viper.setup(Vec2(0, 0), 180.f, seconds(3));
    }

    Viper viper;
};

TEST_F(ViperTest, angleTest) {
    EXPECT_DOUBLE_EQ(viper.angle(), 180.f);
    viper.setAngle(90.f);
    EXPECT_DOUBLE_EQ(viper.angle(), 90.f);
    viper.setAngle(-90.f);
    EXPECT_DOUBLE_EQ(viper.angle(), -90.f);
    viper.setAngle(270.f);
    EXPECT_DOUBLE_EQ(viper.angle(), -90.f);
}

TEST_F(ViperTest, lengthTest) {
    EXPECT_DOUBLE_EQ(viper.length(), 3.f * viper.speed());
}

}  // namespace