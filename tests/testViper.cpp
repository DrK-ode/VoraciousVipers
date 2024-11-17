#include <gtest/gtest.h>

#include <fstream>
#include <memory>
#include <vvipers/Engine/OptionsJSON.hpp>
#include <vvipers/Engine/TextureFileLoader.hpp>
#include <vvipers/Utilities/Time.hpp>
#include <vvipers/GameElements/Viper.hpp>
#include <vvipers/config.hpp>
#include <vvipers/Utilities/debug.hpp>

using namespace VVipers;

namespace {

class ViperTest : public ::testing::Test {
  public:
    ViperTest() {
        debug::verbosity = Verbosity::OnlyErrors;
        std::ifstream input("preferences.json");
        options = std::make_unique<OptionsJSON>(input);
        const std::string resPathOptStr("General/resourceDirectoryPath");
        if (!options->is_option_set(resPathOptStr))
            options->set_option_string(resPathOptStr, RESOURCE_PATH);
        auto textures = std::make_unique<TextureFileLoader>(*options.get());
        viper = std::make_unique<Viper>(*options, *textures);
        viper->setup(Vec2(0, 0), 0.f, 1.5);
    }

    std::unique_ptr<Viper> viper;
    std::unique_ptr<OptionsJSON> options;
};

TEST_F(ViperTest, angleTest) {
    EXPECT_DOUBLE_EQ(viper->angle(), 0.);
    viper->set_angle(90.);
    EXPECT_DOUBLE_EQ(viper->angle(), 90.);
    viper->set_angle(-90.);
    EXPECT_DOUBLE_EQ(viper->angle(), -90.);
    viper->set_angle(270.);
    EXPECT_DOUBLE_EQ(viper->angle(), -90.);
}

TEST_F(ViperTest, velocityTest) {
    EXPECT_DOUBLE_EQ(viper->speed(), viper->velocity().abs());
}

TEST_F(ViperTest, lengthTest) {
    double expectedLength =
        options->option_double("ViperModel/ViperHead/nominalLength") +
        1.5 *
            options->option_double("ViperModel/ViperBody/nominalLength") +
        options->option_double("ViperModel/ViperTail/nominalLength");
    viper->update(time_from_seconds(3.0));  // Let it grow
    EXPECT_DOUBLE_EQ(viper->length(), expectedLength);
    viper->update(time_from_seconds(3.0));  // Let it grow more
    EXPECT_DOUBLE_EQ(viper->length(), expectedLength);
}

}  // namespace