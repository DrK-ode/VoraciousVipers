#include <gtest/gtest.h>

#include <fstream>
#include <string>
#include <memory>
#include <vvipers/Engine/OptionsJSON.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/config.hpp>
#include <vvipers/Utilities/debug.hpp>

using namespace VVipers;

namespace {

TEST(JsonTest, basicReadTest) {
    debug::verbosity = Verbosity::silent;
    std::ifstream input("test.json");
    auto options = std::make_unique<OptionsJSON>(input);

    EXPECT_FALSE(options->isOptionSet("value100"));
    EXPECT_TRUE(options->isOptionSet("value1"));

    EXPECT_DOUBLE_EQ(options->getOptionDouble("value1"), 1);
    EXPECT_THROW(options->getOptionDouble("value2"), Json::LogicError);
    EXPECT_THROW(options->getOptionDouble("value3"), Json::LogicError);

    EXPECT_EQ(options->getOptionString("value1"), "1");
    EXPECT_EQ(options->getOptionString("value2"), "two");
    EXPECT_THROW(options->getOptionString("value3"), Json::LogicError);

    EXPECT_THROW(options->getOption2DVector("value1"), Json::LogicError);
    EXPECT_THROW(options->getOption2DVector("value2"), Json::LogicError);
    EXPECT_EQ(options->getOption2DVector("value3"), Vec2(3, 33));
}

TEST(JsonTest, subdirTest) {
    debug::verbosity = Verbosity::silent;
    std::ifstream input("test.json");
    auto options = std::make_unique<OptionsJSON>(input);

    EXPECT_THROW(options->getOptionDouble("value5"), Json::LogicError);
    EXPECT_THROW(options->getOptionString("value5"), Json::LogicError);
    EXPECT_THROW(options->getOption2DVector("value5"), Json::LogicError);
    EXPECT_EQ(options->getOptionDouble("value5/value51"), 51);
    EXPECT_EQ(options->getOptionDouble("value5/value52/value521"), 521);
}

TEST(JsonTest, arrayTest) {
    debug::verbosity = Verbosity::silent;
    std::ifstream input("test.json");
    auto options = std::make_unique<OptionsJSON>(input);
    std::vector<double> dblArray = {1, 2, 3, 4, 5, 6};
    EXPECT_EQ(options->getOptionDoubleArray("value6"), dblArray);
}

TEST(JsonTest, setTest) {
    debug::verbosity = Verbosity::silent;
    std::ifstream input("test.json");
    auto options = std::make_unique<OptionsJSON>(input);

    std::vector<double> dblArray = {1, 2, 3};
    std::vector<Vec2> vec2Array = {Vec2(1, 11), Vec2(2, 22), Vec2(3, 33)};
    options->setOptionDouble("extra1", 1.0);
    options->setOptionString("extra2", "two");
    options->setOption2DVector("extra3", Vec2(3, 33));
    options->setOptionDoubleArray("extra4", dblArray);
    options->setOption2DVectorArray("extra5", vec2Array);
    EXPECT_DOUBLE_EQ(options->getOptionDouble("extra1"), 1.0);
    EXPECT_EQ(options->getOptionString("extra2"), "two");
    EXPECT_EQ(options->getOption2DVector("extra3"), Vec2(3, 33));
    EXPECT_EQ(options->getOptionDoubleArray("extra4"), dblArray);
    EXPECT_EQ(options->getOption2DVectorArray("extra5"), vec2Array);
}

}  // namespace