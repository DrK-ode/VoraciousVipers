#include <vvipers/Engine/OptionsJSON.hpp>
#include <vvipers/Utilities/debug.hpp>
#include <fstream>

namespace VVipers {

OptionsJSON::OptionsJSON(const std::string& file_path) : _file_path(file_path) {
    std::ifstream input(file_path.c_str());
    Json::Reader reader;
    reader.parse(input, _json_root);
}

std::vector<std::string> tokenize(const std::string& str, const char delim) {
    std::vector<std::string> out;
    size_t start;
    size_t end = 0;

    while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
    return out;
}

const Json::Value OptionsJSON::option_value(
    const std::string& optionName) const {
    auto subdirs = tokenize(optionName, '/');
    // get each subdir before searching for the value
    Json::Value value = _json_root;
    for (auto& subdir : subdirs)
        value = value.get(subdir, Json::nullValue);
    return value;
}

const Json::Value OptionsJSON::option_array(
    const std::string& optionName) const {
    auto value = option_value(optionName);
    if (!value.isArray())
        tag_error(" Value ", optionName, " is not an array.");
    return value;
}

bool OptionsJSON::option_boolean(const std::string& optionName) const {
    auto value = option_value(optionName);
    if (!value.isBool())
        tag_error(" Value ", optionName, " is not a boolean.");
    return value.asBool();
}

double OptionsJSON::option_double(const std::string& optionName) const {
    auto value = option_value(optionName);
    if (!value.isDouble())
        tag_error(" Value ", optionName, " is not a double.");
    return value.asDouble();
}

int OptionsJSON::option_int(const std::string& optionName) const {
    auto value = option_value(optionName);
    if (!value.isInt())
        tag_error(" Value ", optionName, " is not an integer.");
    return value.asInt();
}

std::string OptionsJSON::option_string(const std::string& optionName) const {
    auto value = option_value(optionName);
    if (!value.isString())
        tag_error(" Value ", optionName, " is not a string.");
    return value.asString();
}

Vec2 OptionsJSON::option_2d_vector(const std::string& optionName) const {
    auto value = option_array(optionName);
    if (value.size() != 2 or !value[0].isDouble() or !value[1].isDouble()) {
        tag_error(" Value ", optionName, " is not a 2D vector.");
    }
    return Vec2(value[0].asDouble(), value[1].asDouble());
}

std::vector<bool> OptionsJSON::option_boolean_array(
    const std::string& optionName) const {
    auto value = option_array(optionName);
    std::vector<bool> boolArray;
    boolArray.reserve(value.size());
    for (auto boolValue : value) {
        if (!boolValue.isBool()) {
            tag_error("Expected boolean value in array.");
            continue;
        }
        boolArray.push_back(boolValue.asBool());
    }
    return boolArray;
}

std::vector<double> OptionsJSON::option_double_array(
    const std::string& optionName) const {
    auto value = option_array(optionName);
    std::vector<double> doubleArray;
    doubleArray.reserve(value.size());
    for (auto doubleValue : value) {
        if (!doubleValue.isDouble()) {
            tag_error("Expected double value in array.");
            continue;
        }
        doubleArray.push_back(doubleValue.asDouble());
    }
    return doubleArray;
}

std::vector<int> OptionsJSON::option_int_array(
    const std::string& optionName) const {
    auto value = option_array(optionName);
    std::vector<int> intArray;
    intArray.reserve(value.size());
    for (auto intValue : value) {
        if (!intValue.isInt()) {
            tag_error("Expected int value in array.");
            continue;
        }
        intArray.push_back(intValue.asInt());
    }
    return intArray;
}

std::vector<std::string> OptionsJSON::option_string_array(
    const std::string& optionName) const {
    auto value = option_array(optionName);
    std::vector<std::string> stringArray;
    stringArray.reserve(value.size());
    for (auto stringValue : value) {
        if (!stringValue.isString()) {
            tag_error("Expected string value in array.");
            continue;
        }
        stringArray.push_back(stringValue.asString());
    }
    return stringArray;
}

std::vector<Vec2> OptionsJSON::option_2d_vector_array(
    const std::string& optionName) const {
    auto value = option_array(optionName);
    std::vector<Vec2> vectorArray;
    vectorArray.reserve(value.size());
    for (auto vec2Value : value) {
        if (!vec2Value.isArray() or vec2Value.size() != 2 or
            !vec2Value[0].isDouble() or !vec2Value[1].isDouble()) {
            tag_error("Expected 2D vector value in array.");
            continue;
        }
        vectorArray.push_back(
            {vec2Value[0].asDouble(), vec2Value[1].asDouble()});
    }
    return vectorArray;
}

void OptionsJSON::set_option_value(const std::string& optionName,
                                   const Json::Value optionValue) {
    auto subdirs = tokenize(optionName, '/');
    auto actualName = subdirs.back();
    subdirs.pop_back();
    auto value = &_json_root;
    for (auto& dir : subdirs) {
        value = &(*value)[dir];
    }
    (*value)[actualName] = optionValue;
}

template <typename T>
void OptionsJSON::set_option_array(const std::string& optionName,
                                   const std::vector<T>& optionValue) {
    auto array = Json::Value(Json::arrayValue);
    for (T element : optionValue)
        array.append(element);
    set_option_value(optionName, array);
}

void OptionsJSON::set_option_boolean_array(
    const std::string& optionName, const std::vector<bool>& optionArray) {
    set_option_array(optionName, optionArray);
}

void OptionsJSON::set_option_double_array(
    const std::string& optionName, const std::vector<double>& optionArray) {
    set_option_array(optionName, optionArray);
}

void OptionsJSON::set_option_int_array(const std::string& optionName,
                                       const std::vector<int>& optionArray) {
    set_option_array(optionName, optionArray);
}

void OptionsJSON::set_option_string_array(
    const std::string& optionName,
    const std::vector<std::string>& optionArray) {
    set_option_array(optionName, optionArray);
}

void OptionsJSON::set_option_2d_vector_array(
    const std::string& optionName, const std::vector<Vec2>& vectorArray) {
    auto array = Json::Value(Json::arrayValue);
    for (auto& vec : vectorArray) {
        auto vec2array = Json::Value(Json::arrayValue);
        vec2array.append(vec.x);
        vec2array.append(vec.y);
        array.append(vec2array);
    }
    set_option_value(optionName, array);
}

void OptionsJSON::write() const {
    std::ofstream output(_file_path.c_str());
    output << _json_root << std::endl;
}

}  // namespace VVipers
