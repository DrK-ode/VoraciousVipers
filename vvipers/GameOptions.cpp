#include <vvipers/GameOptions.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

GameOptions* GameOptions::s_instance(nullptr);

GameOptions::GameOptions(std::istream& input) {
    if (s_instance)
        throw std::runtime_error("Only one instance of GameOptions allowed.");
    s_instance = this;

    Json::Reader reader;
    reader.parse(input, m_jsonRoot);
}

GameOptions::~GameOptions() { s_instance = nullptr; }

GameOptions* GameOptions::getInstance() {
    if (s_instance)
        return s_instance;
    throw std::runtime_error("GameOptions not instantiated.");
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

const Json::Value GameOptions::getOptionValue(
    const std::string& optionName) const {
    auto subdirs = tokenize(optionName, '/');
    // get each subdir before searching for the value
    Json::Value value = m_jsonRoot;
    for (auto& subdir : subdirs)
        value = value.get(subdir, Json::nullValue);
    return value;
}

const Json::Value GameOptions::getOptionArray(
    const std::string& optionName) const {
    auto value = getOptionValue(optionName);
    if (!value.isArray())
        tagError(" Value ", optionName, " is not an array.");
    return value;
}

double GameOptions::getOptionDouble(const std::string& optionName) {
    auto value = getInstance()->getOptionValue(optionName);
    if (!value.isDouble())
        tagError(" Value ", optionName, " is not a double.");
    return value.asDouble();
}

std::string GameOptions::getOptionString(const std::string& optionName) {
    auto value = getInstance()->getOptionValue(optionName);
    if (!value.isString())
        tagError(" Value ", optionName, " is not a string.");
    return value.asString();
}

Vec2 GameOptions::getOption2DVector(const std::string& optionName) {
    auto value = getInstance()->getOptionArray(optionName);
    if (value.size() != 2 or !value[0].isDouble() or !value[1].isDouble()) {
        tagError(" Value ", optionName, " is not a 2D vector.");
    }
    return Vec2(value[0].asDouble(), value[1].asDouble());
}

std::vector<std::string> GameOptions::getOptionStringArray(
    const std::string& optionName) {
    auto value = getInstance()->getOptionArray(optionName);
    std::vector<std::string> stringArray;
    stringArray.reserve(value.size());
    for (auto stringValue : value) {
        if (!stringValue.isString()) {
            tagError("Expected string value in array.");
            continue;
        }
        stringArray.push_back(stringValue.asString());
    }
    return stringArray;
}

std::vector<double> GameOptions::getOptionDoubleArray(
    const std::string& optionName) {
    auto value = getInstance()->getOptionArray(optionName);
    std::vector<double> doubleArray;
    doubleArray.reserve(value.size());
    for (auto doubleValue : value) {
        if (!doubleValue.isDouble()) {
            tagError("Expected double value in array.");
            continue;
        }
        doubleArray.push_back(doubleValue.asDouble());
    }
    return doubleArray;
}

std::vector<Vec2> GameOptions::getOption2DVectorArray(
    const std::string& optionName) {
    auto value = getInstance()->getOptionArray(optionName);
    std::vector<Vec2> vectorArray;
    vectorArray.reserve(value.size());
    for (auto vec2Value : value) {
        if (!vec2Value.isArray() or vec2Value.size() != 2 or
            !vec2Value[0].isDouble() or !vec2Value[1].isDouble()) {
            tagError("Expected 2D vector value in array.");
            continue;
        }
        vectorArray.push_back(
            {vec2Value[0].asDouble(), vec2Value[1].asDouble()});
    }
    return vectorArray;
}

void GameOptions::setOptionValue(const std::string& optionName,
                                 const Json::Value optionValue) {
    auto subdirs = tokenize(optionName, '/');
    auto actualName = subdirs.back();
    subdirs.pop_back();
    auto value = &m_jsonRoot;
    for (auto& dir : subdirs) {
        value = &(*value)[dir];
    }
    (*value)[actualName] = optionValue;
}

template <typename T>
void GameOptions::setOptionArray(const std::string& optionName,
                                 const std::vector<T>& optionValue) {
    auto array = Json::Value(Json::arrayValue);
    for (auto& element : optionValue)
        array.append(element);
    setOptionValue(optionName, array);
}

void GameOptions::setOptionDoubleArray(const std::string& optionName,
                                       const std::vector<double>& optionArray) {
    setOptionArray(optionName, optionArray);
}

void GameOptions::setOptionStringArray(
    const std::string& optionName,
    const std::vector<std::string>& optionArray) {
    setOptionArray(optionName, optionArray);
}

void GameOptions::setOption2DVectorArray(const std::string& optionName,
                                         const std::vector<Vec2>& vectorArray) {
    auto array = Json::Value(Json::arrayValue);
    for (auto& vec : vectorArray) {
        auto vec2array = Json::Value(Json::arrayValue);
        vec2array.append(vec.x);
        vec2array.append(vec.y);
        array.append(vec2array);
    }
    setOptionValue(optionName, array);
}

void GameOptions::write(std::ostream& output) {
    output << getInstance()->m_jsonRoot << std::endl;
}

}  // namespace VVipers