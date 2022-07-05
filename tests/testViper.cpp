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
        logLevel = LogLevel::onlyErrors;
        viper.setup(Vec2f(0, 0), 180.f, sf::seconds(3));
    }

    Viper viper;
};

TEST_F(ViperTest, setupTest) {
    EXPECT_THROW(Viper().setup(Vec2f(0, 0), 0.f, sf::seconds(0)), std::runtime_error);
    EXPECT_THROW(Viper().setup(Vec2f(0, 0), 0.f, sf::seconds(1)), std::runtime_error);
    EXPECT_NO_THROW(Viper().setup(Vec2f(0, 0), 0.f, sf::seconds(2)));
}

TEST_F(ViperTest, angleTest) {
    EXPECT_FLOAT_EQ(viper.angle(), 180.f);
    viper.setAngle(90.f);
    EXPECT_FLOAT_EQ(viper.angle(), 90.f);
    viper.setAngle(-90.f);
    EXPECT_FLOAT_EQ(viper.angle(), -90.f);
    viper.setAngle(270.f);
    EXPECT_FLOAT_EQ(viper.angle(), -90.f);
}

TEST_F(ViperTest, lengthTest) {
    EXPECT_FLOAT_EQ(viper.length(), 3.f * viper.speed() );
}

TEST_F(ViperTest, growthTest) {
    // No growth
    /*for( int i = 0; i < 10*viper.getTrackPointsPerSegment(); ++i )
    viper.tick( sf::seconds(1 / 60.0) );
    EXPECT_NEAR(viper.length(), 3.f * ViperVertices::getNominalSegmentLength(), 0.01f );

    viper.growSegment(10);
    for( int i = 0; i < 10*viper.getTrackPointsPerSegment(); ++i )
    viper.tick( sf::seconds(1 / 60.0) );
    EXPECT_NEAR(viper.length() / (13.f * ViperVertices::getNominalSegmentLength()), 1.f, 0.001f);*/
}

}  // namespace