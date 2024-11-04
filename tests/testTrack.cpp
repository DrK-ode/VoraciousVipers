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
        track = std::unique_ptr<TemporalTrack>( new TemporalTrack(Vec2(0, 0), timeFromSeconds(5), Vec2(1, 0), timeFromSeconds(3)) );
        track->create_back(Vec2(1, 1), timeFromSeconds(0));
        track->create_back(Vec2(1, 2), timeFromSeconds(-1));
        track->create_back(Vec2(1, 3), timeFromSeconds(-2));
        track->create_back(Vec2(1, 4), timeFromSeconds(-3));
        track->create_back(Vec2(2, 4), timeFromSeconds(-4));
    }
    std::unique_ptr<TemporalTrack> track;
};

TEST_F(TrackTest, LengthTestBasic) {
    EXPECT_DOUBLE_EQ(track->length(), 6);
    EXPECT_DOUBLE_EQ(track->length(timeFromSeconds(3), timeFromSeconds(0)), 1.);
    EXPECT_DOUBLE_EQ(track->length(timeFromSeconds(4), timeFromSeconds(0)), 1.5);
    EXPECT_DOUBLE_EQ(track->length(timeFromSeconds(5), timeFromSeconds(1)), 1.+2./3.);
}

TEST_F(TrackTest, LengthTestOverFlow) {
    debug::verbosity = VVipers::Verbosity::silent;
    EXPECT_THROW(track->length(timeFromSeconds(6), timeFromSeconds(0)), std::runtime_error);
}

TEST_F(TrackTest, LengthTestInterpolation) {
    EXPECT_DOUBLE_EQ(track->length(timeFromSeconds(5), timeFromSeconds(2)), 1 + 1 / 3.);
}

TEST_F(TrackTest, LengthTestBackwards) {
    EXPECT_THROW(track->length(timeFromSeconds(0), timeFromSeconds(4)), std::runtime_error);
}

TEST_F(TrackTest, DirectionTest) {
    EXPECT_EQ(track->gradient(timeFromSeconds(0)).normalized(), Vec2(0,-1));
    EXPECT_EQ(track->gradient(timeFromSeconds(2)).normalized(), Vec2(0,-1));
    EXPECT_EQ(track->gradient(timeFromSeconds(3)).normalized(), Vec2(-1,0));
    EXPECT_EQ(track->gradient(timeFromSeconds(5)).normalized(), Vec2(-1,0));
}

TEST_F(TrackTest, PositionTest) {
    EXPECT_EQ(track->position(timeFromSeconds(0)), Vec2(1,1));
    EXPECT_EQ(track->position(timeFromSeconds(2)), Vec2(1,1/3.));
    EXPECT_EQ(track->position(timeFromSeconds(3)), Vec2(1,0));
    EXPECT_EQ(track->position(timeFromSeconds(5)), Vec2(0,0));
}

TEST_F(TrackTest, SearchTest) {
    EXPECT_EQ(track->at_or_before(timeFromSeconds(3.), track->head(), track->tail() )->spawn_time, timeFromSeconds(3.) );
    EXPECT_EQ(track->at_or_before(timeFromSeconds(3.5), track->head(), track->tail() )->spawn_time, timeFromSeconds(3.) );
    EXPECT_EQ(track->at_or_before(timeFromSeconds(2.5), track->head(), track->tail() )->spawn_time, timeFromSeconds(0.) );
    EXPECT_EQ(track->at_or_later(timeFromSeconds(3.), track->head(), track->tail() )->spawn_time, timeFromSeconds(3.) );
    EXPECT_EQ(track->at_or_later(timeFromSeconds(3.5), track->head(), track->tail() )->spawn_time, timeFromSeconds(5.) );
    EXPECT_EQ(track->at_or_later(timeFromSeconds(2.5), track->head(), track->tail() )->spawn_time, timeFromSeconds(3.) );

    EXPECT_EQ(track->at_or_before(timeFromSeconds(3.), track->head()+2, track->tail() )->spawn_time, timeFromSeconds(0.) );
    EXPECT_EQ(track->at_or_later(timeFromSeconds(3.), track->head()+2, track->tail() ), track->tail() );
}

}  // namespace