#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <vvipers/Utilities/Time.hpp>
#include <vvipers/GameElements/Track.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace {
using namespace VVipers;
using namespace sf;

class TrackTest : public ::testing::Test {
  protected:
    void SetUp() override {
        track = std::unique_ptr<TemporalTrack>( new TemporalTrack(Vec2(0, 0), timeFromseconds(5), Vec2(1, 0), timeFromseconds(3)) );
        track->create_back(Vec2(1, 1), timeFromseconds(0));
    }
    std::unique_ptr<TemporalTrack> track;
};

TEST_F(TrackTest, LengthTestBasic) {
    EXPECT_DOUBLE_EQ(track->length(), 2);
    EXPECT_DOUBLE_EQ(track->length(timeFromseconds(3), timeFromseconds(0)), 1.);
    EXPECT_DOUBLE_EQ(track->length(timeFromseconds(4), timeFromseconds(0)), 1.5);
    EXPECT_DOUBLE_EQ(track->length(timeFromseconds(5), timeFromseconds(1)), 1.+2./3.);
}

TEST_F(TrackTest, LengthTestOverFlow) {
    debug::verbosity = VVipers::Verbosity::silent;
    EXPECT_THROW(track->length(timeFromseconds(6), timeFromseconds(0)), std::runtime_error);
}

TEST_F(TrackTest, LengthTestInterpolation) {
    EXPECT_DOUBLE_EQ(track->length(timeFromseconds(5), timeFromseconds(2)), 1 + 1 / 3.);
}

TEST_F(TrackTest, LengthTestBackwards) {
    EXPECT_THROW(track->length(timeFromseconds(0), timeFromseconds(4)), std::runtime_error);
}

TEST_F(TrackTest, DirectionTest) {
    EXPECT_EQ(track->gradient(timeFromseconds(0)).normalized(), Vec2(0,-1));
    EXPECT_EQ(track->gradient(timeFromseconds(2)).normalized(), Vec2(0,-1));
    EXPECT_EQ(track->gradient(timeFromseconds(3)).normalized(), Vec2(-1,0));
    EXPECT_EQ(track->gradient(timeFromseconds(5)).normalized(), Vec2(-1,0));
}

TEST_F(TrackTest, PositionTest) {
    EXPECT_EQ(track->position(timeFromseconds(0)), Vec2(1,1));
    EXPECT_EQ(track->position(timeFromseconds(2)), Vec2(1,1/3.));
    EXPECT_EQ(track->position(timeFromseconds(3)), Vec2(1,0));
    EXPECT_EQ(track->position(timeFromseconds(5)), Vec2(0,0));
}

}  // namespace