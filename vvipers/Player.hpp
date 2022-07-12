#ifndef VVIPERS_PLAYER_HPP
#define VVIPERS_PLAYER_HPP

#include <string>
#include <vvipers/Score.hpp>
#include <vvipers/Viper.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

class Player {
  public:
    Player(std::string name);

  private:
    std::string m_name;
    Score m_score;
};

}  // namespace VVipers
#endif