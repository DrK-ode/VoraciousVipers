#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

#include "Controller.hpp"
#include "Score.hpp"
#include "Viper.hpp"
#include "debug.hpp"

class Player {
  public:
    Player(std::string name);
    ~Player();
    void setController(Controller* ctrl){m_controller = ctrl;}
    Controller* getController() const {return m_controller;}

  private:
    Controller* m_controller;
    std::string m_name;
    Score m_score;
};

#endif