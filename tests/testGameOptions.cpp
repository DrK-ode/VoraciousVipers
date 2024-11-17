#include <gtest/gtest.h>

#include <fstream>
#include <memory>
#include <string>
#include <vvipers/Engine/OptionsJSON.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>
#include <vvipers/config.hpp>

using namespace VVipers;

namespace {

TEST(JsonTest, basicReadTest) {
    debug::verbosity = Verbosity::Silent;
    std::ifstream input("test.json");
    auto options = std::make_unique<OptionsJSON>(input);

    EXPECT_FALSE(options->is_option_set("value100"));
    EXPECT_TRUE(options->is_option_set("value1"));

    EXPECT_DOUBLE_EQ(options->option_double("value1"), 1);
    EXPECT_THROW(options->option_double("value2"), Json::LogicError);
    EXPECT_THROW(options->option_double("value3"), Json::LogicError);

    EXPECT_EQ(options->option_string("value1"), "1");
    EXPECT_EQ(options->option_string("value2"), "two");
    EXPECT_THROW(options->option_string("value3"), Json::LogicError);

    EXPECT_THROW(options->option_2d_vector("value1"), Json::LogicError);
    EXPECT_THROW(options->option_2d_vector("value2"), Json::LogicError);
    EXPECT_EQ(options->option_2d_vector("value3"), Vec2(3, 33));
}

TEST(JsonTest, subdirTest) {
    debug::verbosity = Verbosity::Silent;
    std::ifstream input("test.json");
    auto options = std::make_unique<OptionsJSON>(input);

    EXPECT_THROW(options->option_double("value5"), Json::LogicError);
    EXPECT_THROW(options->option_string("value5"), Json::LogicError);
    EXPECT_THROW(options->option_2d_vector("value5"), Json::LogicError);
    EXPECT_EQ(options->option_double("value5/value51"), 51);
    EXPECT_EQ(options->option_double("value5/value52/value521"), 521);
}

TEST(JsonTest, arrayTest) {
    debug::verbosity = Verbosity::Silent;
    std::ifstream input("test.json");
    auto options = std::make_unique<OptionsJSON>(input);
    std::vector<double> dblArray = {1, 2, 3, 4, 5, 6};
    EXPECT_EQ(options->option_double_array("value6"), dblArray);
}

TEST(JsonTest, setTest) {
    debug::verbosity = Verbosity::Silent;
    std::ifstream input("test.json");
    auto options = std::make_unique<OptionsJSON>(input);

    std::vector<double> dblArray = {1, 2, 3};
    std::vector<Vec2> vec2Array = {Vec2(1, 11), Vec2(2, 22), Vec2(3, 33)};
    options->set_option_double("extra1", 1.0);
    options->set_option_string("extra2", "two");
    options->set_option_2d_vector("extra3", Vec2(3, 33));
    options->set_option_double_array("extra4", dblArray);
    options->set_option_2d_vector_array("extra5", vec2Array);
    EXPECT_DOUBLE_EQ(options->option_double("extra1"), 1.0);
    EXPECT_EQ(options->option_string("extra2"), "two");
    EXPECT_EQ(options->option_2d_vector("extra3"), Vec2(3, 33));
    EXPECT_EQ(options->option_double_array("extra4"), dblArray);
    EXPECT_EQ(options->option_2d_vector_array("extra5"), vec2Array);
}

}  // namespace