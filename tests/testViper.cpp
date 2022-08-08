#include <gtest/gtest.h>

#include <fstream>
#include <memory>
#include <vvipers/Engine/OptionsJSON.hpp>
#include <vvipers/Engine/TextureFileLoader.hpp>
#include <vvipers/Utilities/Time.hpp>
#include <vvipers/Scenes/GameElements/Viper.hpp>
#include <vvipers/config.hpp>
#include <vvipers/Utilities/debug.hpp>

using namespace VVipers;

namespace {

class ViperTest : public ::testing::Test {
  public:
    ViperTest() {
        debug::verbosity = Verbosity::onlyErrors;
        std::ifstream input("preferences.json");
        options = std::make_unique<OptionsJSON>(input);
        const std::string resPathOptStr("General/resourceDirectoryPath");
        if (!options->isOptionSet(resPathOptStr))
            options->setOptionString(resPathOptStr, RESOURCE_PATH);
        auto textures = std::make_unique<TextureFileLoader>(*options.get());
        viper = std::make_unique<Viper>(*options, *textures);
        viper->setup(Vec2(0, 0), 180.f, 1.5);
    }

    std::unique_ptr<Viper> viper;
    std::unique_ptr<OptionsJSON> options;
};

TEST_F(ViperTest, angleTest) {
    EXPECT_DOUBLE_EQ(viper->getAngle(), 180.);
    viper->setAngle(90.);
    EXPECT_DOUBLE_EQ(viper->getAngle(), 90.);
    viper->setAngle(-90.);
    EXPECT_DOUBLE_EQ(viper->getAngle(), -90.);
    viper->setAngle(270.);
    EXPECT_DOUBLE_EQ(viper->getAngle(), -90.);
}

TEST_F(ViperTest, velocityTest) {
    EXPECT_DOUBLE_EQ(viper->getSpeed(), viper->getVelocity().abs());
}

TEST_F(ViperTest, lengthTest) {
    double expectedLength =
        options->getOptionDouble("ViperModel/ViperHead/nominalLength") +
        1.5 *
            options->getOptionDouble("ViperModel/ViperBody/nominalLength") +
        options->getOptionDouble("ViperModel/ViperTail/nominalLength");
    viper->update(seconds(3.0));  // Let it grow
    EXPECT_DOUBLE_EQ(viper->getLength(), expectedLength);
    viper->update(seconds(3.0));  // Let it grow more
    EXPECT_DOUBLE_EQ(viper->getLength(), expectedLength);
}

}  // namespace