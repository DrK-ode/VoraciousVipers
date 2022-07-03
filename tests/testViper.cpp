#include <gtest/gtest.h>

#include <SFML/System/Time.hpp>

#include <vvipers/Viper.hpp>
#include <vvipers/ViperVertices.hpp>
#include <vvipers/debug.hpp>

using namespace VVipers;

namespace {

class ViperTest : public ::testing::Test {
  protected:
    void SetUp() override {
        logLevel = LogLevel::all;
        viper.setup(Vec2f(0, 0), 180.f, 3);
    }

    Viper viper;
};

TEST_F(ViperTest, setupTest) {
    EXPECT_THROW(Viper().setup(Vec2f(0, 0), 0.f, 0), std::out_of_range);
    EXPECT_THROW(Viper().setup(Vec2f(0, 0), 0.f, 1), std::out_of_range);
    EXPECT_NO_THROW(Viper().setup(Vec2f(0, 0), 0.f, 2));
}

TEST_F(ViperTest, angleTest) {
    EXPECT_FLOAT_EQ(viper.getAngle(), 180.f);
    viper.setAngle(90.f);
    EXPECT_FLOAT_EQ(viper.getAngle(), 90.f);
    viper.setAngle(-90.f);
    EXPECT_FLOAT_EQ(viper.getAngle(), -90.f);
    viper.setAngle(270.f);
    EXPECT_FLOAT_EQ(viper.getAngle(), -90.f);
}

TEST_F(ViperTest, lengthTest) {
    EXPECT_FLOAT_EQ(Viper().length(), 0.f);
    EXPECT_FLOAT_EQ(viper.length(), 3.f * ViperVertices::getNominalSegmentLength());
}

TEST_F(ViperTest, growthTest) {
    // No growth
    for( int i = 0; i < 10*viper.getTrackPointsPerSegment(); ++i )
    viper.tick( sf::seconds(1 / 60.0) );
    EXPECT_NEAR(viper.length(), 3.f * ViperVertices::getNominalSegmentLength(), 0.01f );

    viper.growSegment(10);
    for( int i = 0; i < 10*viper.getTrackPointsPerSegment(); ++i )
    viper.tick( sf::seconds(1 / 60.0) );
    EXPECT_NEAR(viper.length() / (13.f * ViperVertices::getNominalSegmentLength()), 1.f, 0.001f);
}

}  // namespace