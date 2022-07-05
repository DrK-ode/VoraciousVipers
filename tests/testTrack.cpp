#include <gtest/gtest.h>

#include <SFML/System/Time.hpp>
#include <vvipers/Track.hpp>

using namespace VVipers;

namespace {

using namespace sf;

class TrackTest : public ::testing::Test {
  protected:
    void SetUp() override {
        track.create_back(Vec2f(0, 0), seconds(5));
        track.create_back(Vec2f(1, 0), seconds(3));
        track.create_back(Vec2f(2, 0), seconds(0));
    }
    Track track;
};

TEST_F(TrackTest, LengthTestBasic) {
    EXPECT_FLOAT_EQ(track.length(), 2);
    EXPECT_FLOAT_EQ(track.length(seconds(3), seconds(0)), 1);
    EXPECT_FLOAT_EQ(track.length(seconds(4), seconds(0)), 1.5);
}

TEST_F(TrackTest, LengthTestOverFlow) {
    EXPECT_THROW(track.length(seconds(6), seconds(0)), std::runtime_error);
}

TEST_F(TrackTest, LengthTestInterpolation) {
    EXPECT_FLOAT_EQ(track.length(seconds(5), seconds(2)), 1 + 1 / 3.f);
}

TEST_F(TrackTest, LengthTestBackwards) {
    EXPECT_FLOAT_EQ(track.length(seconds(0), seconds(4)), -1.5);
}

}  // namespace