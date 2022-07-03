#include <gtest/gtest.h>

namespace {

TEST(HelloTest, TestingGTest) {
    EXPECT_NE(int(1), int(0.0));
    EXPECT_EQ(int(0), int(0.0));
}

}  // namespace