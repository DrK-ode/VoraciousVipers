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
    static std::vector<std::string> getOptionStringArray(
        const std::string& optionName);
    static std::vector<double> getOptionDoubleArray(
        const std::string& optionName);
    static std::vector<Vec2> getOption2DVectorArray(
        const std::string& optionName);
    static void write(std::ostream& output);

    void setOptionString(const std::string& optionName,
                         const std::string& value) {
        setOption(optionName, value);
    }
    void setOptionDouble(const std::string& optionName, double value) {
        setOption(optionName, value);
    }
    void setOption2DVector(const std::string& optionName, Vec2 value) {
        setOptionDoubleArray(optionName,
                             std::vector<double>({value.x, value.y}));
    }
    void setOptionStringArray(const std::string& optionName,
                              const std::vector<std::string>& stringArray) {
        setOptionArray(optionName, stringArray);
    }
    void setOptionDoubleArray(const std::string& optionName,
                              const std::vector<double>& doubleArray) {
        setOptionArray(optionName, doubleArray);
    }

  private:
    static GameOptions* getInstance();
    Json::Value getOption(const Json::Value& root,
                          const std::string& optionName) const;
    Json::Value getOption(const std::string& optionName) const;
    Json::Value getOptionArray(const Json::Value& root,
                               const std::string& optionName) const;
    Json::Value getOptionArray(const std::string& optionName) const;
    template <typename T>
    void setOption(const std::string& optionName, const T value);
    template <typename T>
    void setOptionArray(const std::string& optionName,
                        const std::vector<T>& value);

    static GameOptions* s_instance;
    Json::Value m_jsonRoot;
};

}  // namespace VVipers

#endif  // VVIPERS_GAMEOPTIONS_HPP
