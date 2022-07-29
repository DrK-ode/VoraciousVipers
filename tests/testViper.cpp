#include <gtest/gtest.h>

#include <fstream>
#include <vvipers/GameOptions.hpp>
#include <vvipers/Time.hpp>
#include <vvipers/Viper.hpp>
#include <vvipers/config.hpp>
#include <vvipers/debug.hpp>

using namespace VVipers;

namespace {

class ViperTest : public ::testing::Test {
  public:
    ViperTest() {
        debug::verbosity = Verbosity::onlyErrors;
        std::ifstream input("test.json");
        options = new GameOptions(input);
        const std::string resPathOptStr("General/resourceDirectoryPath");
        if (!options->isOptionSet(resPathOptStr))
            options->setOptionString(resPathOptStr, RESOURCE_PATH);
        viper = new Viper;
        viper->setup(Vec2(0, 0), 180.f, 1.5);
    }
    ~ViperTest() {
        delete options;
        delete viper;
    }

    Viper* viper;
    GameOptions* options;
};

TEST_F(ViperTest, angleTest) {
    EXPECT_DOUBLE_EQ(viper->angle(), 180.);
    viper->angle(90.);
    EXPECT_DOUBLE_EQ(viper->angle(), 90.);
    viper->angle(-90.);
    EXPECT_DOUBLE_EQ(viper->angle(), -90.);
    viper->angle(270.);
    EXPECT_DOUBLE_EQ(viper->angle(), -90.);
}

TEST_F(ViperTest, velocityTest) {
    EXPECT_DOUBLE_EQ(viper->speed(), viper->velocity().abs());
}

TEST_F(ViperTest, lengthTest) {
    double expectedLength =
        GameOptions::getOptionDouble("ViperModel/ViperHead/nominalLength") +
        1.5 *
            GameOptions::getOptionDouble("ViperModel/ViperBody/nominalLength") +
        GameOptions::getOptionDouble("ViperModel/ViperTail/nominalLength");
    viper->update(seconds(3.0));  // Let it grow
    EXPECT_DOUBLE_EQ(viper->length(), expectedLength);
    viper->update(seconds(3.0));  // Let it grow more
    EXPECT_DOUBLE_EQ(viper->length(), expectedLength);
}

}  // namespace