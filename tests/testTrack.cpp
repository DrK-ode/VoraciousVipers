#include <gtest/gtest.h>

#include <vvipers/Utilities/Time.hpp>
#include <vvipers/Scenes/GameElements/Track.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace {
using namespace VVipers;
using namespace sf;

class TrackTest : public ::testing::Test {
  protected:
    void SetUp() override {
        track.create_back(Vec2(0, 0), seconds(5));
        track.create_back(Vec2(1, 0), seconds(3));
        track.create_back(Vec2(1, 1), seconds(0));
    }
    Track track;
};

TEST_F(TrackTest, LengthTestBasic) {
    EXPECT_DOUBLE_EQ(track.length(), 2);
    EXPECT_DOUBLE_EQ(track.length(seconds(3), seconds(0)), 1);
    EXPECT_DOUBLE_EQ(track.length(seconds(4), seconds(0)), 1.5);
}

TEST_F(TrackTest, LengthTestOverFlow) {
    debug::verbosity = VVipers::Verbosity::silent;
    EXPECT_THROW(track.length(seconds(6), seconds(0)), std::runtime_error);
}

TEST_F(TrackTest, LengthTestInterpolation) {
    EXPECT_DOUBLE_EQ(track.length(seconds(5), seconds(2)), 1 + 1 / 3.);
}

TEST_F(TrackTest, LengthTestBackwards) {
    EXPECT_DOUBLE_EQ(track.length(seconds(0), seconds(4)), -1.5);
}

TEST_F(TrackTest, DirectionTest) {
    EXPECT_EQ(track.gradient(seconds(0)).normalized(), Vec2(0,-1));
    EXPECT_EQ(track.gradient(seconds(2)).normalized(), Vec2(0,-1));
    EXPECT_EQ(track.gradient(seconds(3)).normalized(), Vec2(-1,0));
    EXPECT_EQ(track.gradient(seconds(5)).normalized(), Vec2(-1,0));
}

TEST_F(TrackTest, PositionTest) {
    EXPECT_EQ(track.position(seconds(0)), Vec2(1,1));
    EXPECT_EQ(track.position(seconds(2)), Vec2(1,1/3.));
    EXPECT_EQ(track.position(seconds(3)), Vec2(1,0));
    EXPECT_EQ(track.position(seconds(5)), Vec2(0,0));
}

}  // namespace