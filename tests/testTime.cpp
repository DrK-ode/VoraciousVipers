#include <gtest/gtest.h>

#include <chrono>
#include <thread>
#include <vvipers/Utilities/Time.hpp>

using namespace VVipers;
using namespace std::chrono_literals;

namespace {

TEST(TimeTest, BasicTest) {
    EXPECT_EQ(Time(0.), timeFromSeconds(0.));
    EXPECT_EQ(timeAsSeconds(Time(1.0)), 1.0);
    EXPECT_EQ(timeFromSeconds(1.0) / timeFromSeconds(2.0), 0.5);
}

TEST(TimeTest, StopwatchTest) {
    Stopwatch clock;
    EXPECT_THROW(clock.stop(), std::logic_error);
    EXPECT_THROW(clock.split(), std::logic_error);
    clock.start();
    std::this_thread::sleep_for(std::chrono::duration<double>(1 / 60.));
    auto split = clock.split();
    std::this_thread::sleep_for(std::chrono::duration<double>(1 / 60.));
    auto duration = clock.stop();
    // Resolution typically worse than 5% appearently...
    EXPECT_NEAR(timeAsSeconds(split) * 60, 1, 0.1);
    EXPECT_NEAR(timeAsSeconds(duration) * 60, 2, 0.1);
    EXPECT_THROW(clock.stop(), std::logic_error);
}

}  // namespace