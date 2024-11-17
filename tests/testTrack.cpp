#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <vvipers/GameElements/Track.hpp>
#include <vvipers/Utilities/Time.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace {
using namespace VVipers;
using namespace sf;

class TrackTest : public ::testing::Test {
  protected:
    void SetUp() override {
        /*track = std::unique_ptr<TemporalTrack>( new TemporalTrack(Vec2(0, 0),
        timeFromSeconds(5), Vec2(1, 0), timeFromSeconds(3)) );
        track->create_back(Vec2(1, 1), timeFromSeconds(0));
        track->create_back(Vec2(1, 2), timeFromSeconds(-1));
        track->create_back(Vec2(1, 3), timeFromSeconds(-2));
        track->create_back(Vec2(1, 4), timeFromSeconds(-3));
        track->create_back(Vec2(2, 4), timeFromSeconds(-4));*/

        track = std::unique_ptr<TemporalTrack>(new TemporalTrack(
            Vec2(1, 4), time_from_seconds(-3), Vec2(2, 4), time_from_seconds(-4)));
        track->create_front(Vec2(0, -1), time_from_seconds(1.));
        track->create_front(Vec2(0, -1), time_from_seconds(1.));
        track->create_front(Vec2(0, -1), time_from_seconds(1.));
        track->create_front(Vec2(0, -1. / 3.), time_from_seconds(3.));
        track->create_front(Vec2(-1. / 2., 0), time_from_seconds(2.));
    }
    std::unique_ptr<TemporalTrack> track;
};

TEST_F(TrackTest, LengthTestBasic) {
    EXPECT_DOUBLE_EQ(track->length(), 6);
    EXPECT_DOUBLE_EQ(track->length(time_from_seconds(3), time_from_seconds(0)), 1.);
    // EXPECT_DOUBLE_EQ(track->length(timeFromSeconds(4),
    // timeFromSeconds(0)), 1.5);
    // EXPECT_DOUBLE_EQ(track->length(timeFromSeconds(5),
    // timeFromSeconds(1)), 1.+2./3.);
}

TEST_F(TrackTest, LengthTestOverFlow) {
    debug::verbosity = VVipers::Verbosity::Silent;
    EXPECT_THROW(track->length(time_from_seconds(6), time_from_seconds(0)),
                 std::runtime_error);
}

TEST_F(TrackTest, LengthTestInterpolation) {
    EXPECT_DOUBLE_EQ(track->length(time_from_seconds(5), time_from_seconds(2)),
                     1 + 1 / 3.);
}

TEST_F(TrackTest, LengthTestBackwards) {
    EXPECT_THROW(track->length(time_from_seconds(0), time_from_seconds(4)),
                 std::runtime_error);
}

TEST_F(TrackTest, DirectionTest) {
    EXPECT_EQ(track->velocity(time_from_seconds(0)).normalized(), Vec2(0, -1));
    EXPECT_EQ(track->velocity(time_from_seconds(2)).normalized(), Vec2(0, -1));
    EXPECT_EQ(track->velocity(time_from_seconds(3)).normalized(), Vec2(-1, 0));
    EXPECT_EQ(track->velocity(time_from_seconds(5)).normalized(), Vec2(-1, 0));
}

TEST_F(TrackTest, PositionTest) {
    EXPECT_EQ(track->position(time_from_seconds(0)), Vec2(1, 1));
    EXPECT_DOUBLE_EQ(track->position(time_from_seconds(2)).x, 1.);
    EXPECT_DOUBLE_EQ(track->position(time_from_seconds(2)).y, 1./3.);
    EXPECT_EQ(track->position(time_from_seconds(3)), Vec2(1, 0));
    EXPECT_EQ(track->position(time_from_seconds(5)), Vec2(0, 0));
}

TEST_F(TrackTest, SearchTest) {
    EXPECT_EQ(
        track->at_or_before(time_from_seconds(3.), track->begin(), track->end())
            ->spawn_time,
        time_from_seconds(3.));
    EXPECT_EQ(
        track->at_or_before(time_from_seconds(3.5), track->begin(), track->end())
            ->spawn_time,
        time_from_seconds(3.));
    EXPECT_EQ(
        track->at_or_before(time_from_seconds(2.5), track->begin(), track->end())
            ->spawn_time,
        time_from_seconds(0.));
    EXPECT_EQ(
        track->at_or_later(time_from_seconds(3.), track->begin(), track->end())
            ->spawn_time,
        time_from_seconds(3.));
    EXPECT_EQ(
        track->at_or_later(time_from_seconds(3.5), track->begin(), track->end())
            ->spawn_time,
        time_from_seconds(5.));
    EXPECT_EQ(
        track->at_or_later(time_from_seconds(2.5), track->begin(), track->end())
            ->spawn_time,
        time_from_seconds(3.));

    EXPECT_EQ(track->at_or_before(time_from_seconds(-10.), track->begin(),
                                  track->end()),
              track->end());
    EXPECT_EQ(
        track->at_or_later(time_from_seconds(10.), track->begin(), track->end()),
        track->end());

    EXPECT_EQ(track
                  ->at_or_before(time_from_seconds(3.), track->begin() + 2,
                                 track->end())
                  ->spawn_time,
              time_from_seconds(0.));
    EXPECT_EQ(track->at_or_later(time_from_seconds(3.), track->begin() + 2,
                                 track->end()),
              track->end());

    EXPECT_EQ(track->at_or_later(time_from_seconds(3.), track->begin(),
                                 track->begin() + 2),
              track->begin() + 1);
}

}  // namespace