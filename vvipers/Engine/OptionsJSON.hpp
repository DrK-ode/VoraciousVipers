#pragma once

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

    bool option_boolean(const std::string& optionName) const override;
    std::string option_string(const std::string& optionName) const override;
    double option_double(const std::string& optionName) const override;
    int option_int(const std::string& optionName) const override;
    Vec2 option_2d_vector(const std::string& optionName) const override;
    std::vector<bool> option_boolean_array(
        const std::string& optionName) const override;
    std::vector<double> option_double_array(
        const std::string& optionName) const override;
    std::vector<int> option_int_array(
        const std::string& optionName) const override;
    std::vector<std::string> option_string_array(
        const std::string& optionName) const override;
    std::vector<Vec2> option_2d_vector_array(
        const std::string& optionName) const override;
    bool is_option_set(const std::string& optionName) const override {
        return !option_value(optionName).isNull();
    }
    void write(std::ostream& output) const override;

    void set_option_boolean(const std::string& optionName,
                            bool optionValue) override {
        set_option_value(optionName, Json::Value(optionValue));
    }
    void set_option_double(const std::string& optionName,
                           double optionValue) override {
        set_option_value(optionName, Json::Value(optionValue));
    }
    void set_option_int(const std::string& optionName,
                           int optionValue) override {
        set_option_value(optionName, Json::Value(optionValue));
    }
    void set_option_string(const std::string& optionName,
                           const std::string& optionValue) override {
        set_option_value(optionName, Json::Value(optionValue));
    }
    void set_option_2d_vector(const std::string& optionName,
                              Vec2 value) override {
        set_option_double_array(optionName,
                                std::vector<double>({value.x, value.y}));
    }
    void set_option_boolean_array(
        const std::string& optionName,
        const std::vector<bool>& booleanArray) override;
    void set_option_double_array(
        const std::string& optionName,
        const std::vector<double>& doubleArray) override;
    void set_option_int_array(
        const std::string& optionName,
        const std::vector<int>& intArray) override;
    void set_option_string_array(
        const std::string& optionName,
        const std::vector<std::string>& stringArray) override;
    void set_option_2d_vector_array(
        const std::string& optionName,
        const std::vector<Vec2>& vectorArray) override;

  private:
    const Json::Value option_value(const std::string& optionName) const;
    const Json::Value option_array(const std::string& optionName) const;

    void set_option_value(const std::string& optionName,
                          const Json::Value value);
    template <typename T>
    void set_option_array(const std::string& optionName,
                          const std::vector<T>& value);

    Json::Value _json_root;
};

}  // namespace VVipers
