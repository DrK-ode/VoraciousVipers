#ifndef VVIPERS_OPTIONSJSON_HPP
#define VVIPERS_OPTIONSJSON_HPP

#include <json/json.h>

#include <iostream>
#include <string>
#include <vector>
#include <vvipers/Engine/Providers.hpp>
#include <vvipers/Utilities/Vec2.hpp>

namespace VVipers {

class OptionsJSON : public OptionsProvider {
  public:
    OptionsJSON(std::istream& input);
    virtual ~OptionsJSON() {}

    bool getOptionBoolean(const std::string& optionName) const override;
    std::string getOptionString(const std::string& optionName) const override;
    double getOptionDouble(const std::string& optionName) const override;
    Vec2 getOption2DVector(const std::string& optionName) const override;
    std::vector<bool> getOptionBooleanArray(
        const std::string& optionName) const override;
    std::vector<double> getOptionDoubleArray(
        const std::string& optionName) const override;
    std::vector<std::string> getOptionStringArray(
        const std::string& optionName) const override;
    std::vector<Vec2> getOption2DVectorArray(
        const std::string& optionName) const override;
    bool isOptionSet(const std::string& optionName) const override {
        return !getOptionValue(optionName).isNull();
    }
    void write(std::ostream& output) const override;

    void setOptionBoolean(const std::string& optionName,
                          bool optionValue) override {
        setOptionValue(optionName, Json::Value(optionValue));
    }
    void setOptionDouble(const std::string& optionName,
                         double optionValue) override {
        setOptionValue(optionName, Json::Value(optionValue));
    }
    void setOptionString(const std::string& optionName,
                         const std::string& optionValue) override {
        setOptionValue(optionName, Json::Value(optionValue));
    }
    void setOption2DVector(const std::string& optionName, Vec2 value) override {
        setOptionDoubleArray(optionName,
                             std::vector<double>({value.x, value.y}));
    }
    void setOptionBooleanArray(const std::string& optionName,
                               const std::vector<bool>& booleanArray) override;
    void setOptionDoubleArray(const std::string& optionName,
                              const std::vector<double>& doubleArray) override;
    void setOptionStringArray(
        const std::string& optionName,
        const std::vector<std::string>& stringArray) override;
    void setOption2DVectorArray(const std::string& optionName,
                                const std::vector<Vec2>& vectorArray) override;

  private:
    const Json::Value getOptionValue(const std::string& optionName) const;
    const Json::Value getOptionArray(const std::string& optionName) const;

    void setOptionValue(const std::string& optionName, const Json::Value value);
    template <typename T>
    void setOptionArray(const std::string& optionName,
                        const std::vector<T>& value);

    Json::Value m_jsonRoot;
};

}  // namespace VVipers

#endif  // VVIPERS_OPTIONSJSON_HPP
