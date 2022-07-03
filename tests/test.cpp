#include <gtest/gtest.h>

namespace {

TEST(HelloTest, TestingGTestSuccess) {
    EXPECT_NE(int(1), int(0.0));
    EXPECT_EQ(int(0), int(0.0));
}
TEST(HelloTest, TestingGTestFail) {
    EXPECT_NE(int(0), int(0.0));
    EXPECT_EQ(int(1), int(0.0));
}

}  // namespace