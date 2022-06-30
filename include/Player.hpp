#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

#include "Controller.hpp"
#include "Score.hpp"
#include "Viper.hpp"
#include "debug.hpp"

class Player {
  public:
    Player();
    ~Player();

  private:
    Controller* m_controller;
    std::string m_name;
    Score m_score;
    Viper m_viper;
}

#endif