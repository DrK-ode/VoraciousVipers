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

Json::Value GameOptions::getOption(const Json::Value& root,
                                   const std::string& optionName) const {
    auto subdirs = tokenize(optionName, '/');
    // get each subdir before searching for the value
    Json::Value value = root;
    for (auto& subdir : subdirs)
        value = value.get(subdir, Json::nullValue);
    if (value.isNull())
        tagWarning(" Value ", optionName, " not found among game options.");
    return value;
}

Json::Value GameOptions::getOption(const std::string& optionName) const {
    return getOption(m_jsonRoot, optionName);
}

Json::Value GameOptions::getOptionArray(const Json::Value& root,
                                        const std::string& optionName) const {
    auto value = getOption(root, optionName);
    if (!value.isArray())
        tagError(" Value ", optionName, " is not an array.");
    return value;
}

Json::Value GameOptions::getOptionArray(const std::string& optionName) const {
    return getOptionArray(m_jsonRoot, optionName);
}

double GameOptions::getOptionDouble(const std::string& optionName) {
    auto value = getInstance()->getOption(optionName);
    if (!value.isDouble())
        tagError(" Value ", optionName, " is not a double.");
    return value.asDouble();
}

std::string GameOptions::getOptionString(const std::string& optionName) {
    auto value = getInstance()->getOption(optionName);
    if (!value.isString())
        tagError(" Value ", optionName, " is not a string.");
    return value.asString();
}

Vec2 GameOptions::getOption2DVector(const std::string& optionName) {
    auto value = getInstance()->getOption(optionName);
    if (!value.isArray() or value.size() != 2 or !value[0].isDouble() or
        !value[1].isDouble()) {
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

template <typename T>
void GameOptions::setOption(const std::string& optionName,
                            const T optionValue) {
    auto subdirs = tokenize(optionName, '/');
    auto actualName = subdirs.back();
    subdirs.pop_back();
    auto value = &m_jsonRoot;
    for (auto& dir : subdirs) {
        value = &(*value)[dir];
    }
    (*value)[actualName] = Json::Value(optionValue);
}
template void GameOptions::setOption<double>(const std::string& optionName,
                                             const double optionValue);
template void GameOptions::setOption<std::string>(
    const std::string& optionName, const std::string optionValue);

void GameOptions::write(std::ostream& output) {
    output << getInstance()->m_jsonRoot << std::endl;
}

}  // namespace VVipers