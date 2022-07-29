#ifndef VVIPERS_GAMEOPTIONS_HPP
#define VVIPERS_GAMEOPTIONS_HPP

#include <json/json.h>

#include <iostream>
#include <string>
#include <vector>
#include <vvipers/Vec2.hpp>

namespace VVipers {

class GameOptions {
  public:
    GameOptions(std::istream& input);
    ~GameOptions();

    static std::string getOptionString(const std::string& optionName);
    static double getOptionDouble(const std::string& optionName);
    static Vec2 getOption2DVector(const std::string& optionName);
    static std::vector<double> getOptionDoubleArray(
        const std::string& optionName);
    static std::vector<std::string> getOptionStringArray(
        const std::string& optionName);
    static std::vector<Vec2> getOption2DVectorArray(
        const std::string& optionName);
    static void write(std::ostream& output);

    /* Non-static, requires the actual object */
    void setOptionDouble(const std::string& optionName, double optionValue) {
        setOptionValue(optionName, Json::Value(optionValue));
    }
    void setOptionString(const std::string& optionName,
                         const std::string& optionValue) {
        setOptionValue(optionName, Json::Value(optionValue));
    }
    void setOption2DVector(const std::string& optionName, Vec2 value) {
        setOptionDoubleArray(optionName,
                             std::vector<double>({value.x, value.y}));
    }
    void setOptionStringArray(const std::string& optionName,
                              const std::vector<std::string>& stringArray);
    void setOptionDoubleArray(const std::string& optionName,
                              const std::vector<double>& doubleArray);
    void setOption2DVectorArray(const std::string& optionName,
                                const std::vector<Vec2>& vectorArray);

  private:
    static GameOptions* getInstance();

    const Json::Value getOptionValue(const std::string& optionName) const;
    const Json::Value getOptionArray(const std::string& optionName) const;

    void setOptionValue(const std::string& optionName, const Json::Value value);
    template <typename T>
    void setOptionArray(const std::string& optionName,
                        const std::vector<T>& value);

    static GameOptions* s_instance;
    Json::Value m_jsonRoot;
};

}  // namespace VVipers

#endif  // VVIPERS_GAMEOPTIONS_HPP
