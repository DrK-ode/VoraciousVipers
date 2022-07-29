#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <vvipers/debug.hpp>
#include <vvipers/GameOptions.hpp>
#include <vvipers/Vec2.hpp>

using namespace VVipers;

namespace {

class JsonTest : public ::testing::Test {
  public:
    JsonTest() {
        debug::verbosity = Verbosity::silent;
        std::ifstream input("test.json");
        options = new GameOptions(input);
    }
    ~JsonTest(){
        delete options;
    }

    GameOptions* options;
};

TEST_F(JsonTest, basicReadTest) {
    EXPECT_DOUBLE_EQ( options->getOptionDouble("value1") , 1);
    EXPECT_THROW( options->getOptionDouble("value2") , Json::LogicError);
    EXPECT_THROW( options->getOptionDouble("value3"), Json::LogicError );

    EXPECT_EQ( options->getOptionString("value1") , "1");
    EXPECT_EQ( options->getOptionString("value2") , "two");
    EXPECT_THROW( options->getOptionString("value3"), Json::LogicError);

    EXPECT_THROW( options->getOption2DVector("value1") , Json::LogicError);
    EXPECT_THROW( options->getOption2DVector("value2") , Json::LogicError);
    EXPECT_EQ( options->getOption2DVector("value3"), Vec2(3,33) );
}

TEST_F(JsonTest, subdirTest) {
    EXPECT_THROW( options->getOptionDouble("value5"), Json::LogicError );
    EXPECT_THROW( options->getOptionString("value5"), Json::LogicError );
    EXPECT_THROW( options->getOption2DVector("value5"), Json::LogicError );
    EXPECT_EQ( options->getOptionDouble("value5/value51"), 51 );
    EXPECT_EQ( options->getOptionDouble("value5/value52/value521"), 521 );
}

}  // namespace