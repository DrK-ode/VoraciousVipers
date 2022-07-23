#ifndef VVIPERS_GAMECONFIGURATION_HPP
#define VVIPERS_GAMECONFIGURATION_HPP

#include <json/json.h>

#include <string>
#include <vvipers/Vec2.hpp>

namespace VVipers {

class GameConfiguration {
  public:
    GameConfiguration(const std::string& configurationFilename) {
        read(configurationFilename);
    }

    int getValueAsInt(const std::string& name) const;
    double getValueAsDouble(const std::string& name) const;
    std::string getValueAsString(const std::string& name) const;
    Vec2 getValueAsVec2(const std::string& name) const;

  private:
    void read( const std::string& configurationFilename);

    Json::Value m_jsonValue;
};

}  // namespace VVipers

#endif  // VVIPERS_GAMECONFIGURATION_HPP
