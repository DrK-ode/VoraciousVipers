#include <gtest/gtest.h>

#include <chrono>
#include <thread>
#include <vvipers/Time.hpp>

using namespace VVipers;
using namespace std::chrono_literals;

namespace {

TEST(TimeTest, BasicTest) {
    EXPECT_EQ(Time(0.), seconds(0.));
    EXPECT_EQ(toSeconds(Time(1.0)), 1.0);
    EXPECT_EQ(seconds(1.0) / seconds(2.0), 0.5);
}

TEST(TimeTest, StopwatchTest) {
    StopWatch clock;
    EXPECT_THROW(clock.stop(), std::logic_error);
    EXPECT_THROW(clock.split(), std::logic_error);
    clock.start();
    std::this_thread::sleep_for(std::chrono::duration<double>(1 / 60.));
    auto split = clock.split();
    std::this_thread::sleep_for(std::chrono::duration<double>(1 / 60.));
    auto duration = clock.stop();
    // Cannot expect much better resolution than 1%
    EXPECT_NEAR(toSeconds(split) * 60, 1, 0.01);
    EXPECT_NEAR(toSeconds(duration) * 60, 2, 0.01);
    EXPECT_THROW(clock.stop(), std::logic_error);
}

}  // namespace