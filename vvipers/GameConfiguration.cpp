#include <vvipers/GameConfiguration.hpp>
#include <fstream>

namespace VVipers{

void GameConfiguration::read( const std::string& configurationFilename){
    std::ifstream inputStream(configurationFilename);
    Json::Reader reader;
    reader.parse(inputStream, m_jsonValue, true);
}

int GameConfiguration::getValueAsInt(const std::string& name) const{
    return m_jsonValue[name.c_str()].asInt();
}

double GameConfiguration::getValueAsDouble(const std::string& name) const{
    return m_jsonValue[name.c_str()].asDouble();
}

std::string GameConfiguration::getValueAsString(const std::string& name) const{
    return m_jsonValue[name.c_str()].asString();
}

Vec2 GameConfiguration::getValueAsVec2(const std::string& name) const{
    auto vector = m_jsonValue[name.c_str()];
    return Vec2( vector["x"].asDouble(), vector["y"].asDouble() );
}

}