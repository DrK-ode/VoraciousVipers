#ifndef VVIPERS_PLAYER_HPP
#define VVIPERS_PLAYER_HPP

#include <string>

#include <vvipers/Controller.hpp>
#include <vvipers/Score.hpp>
#include <vvipers/Viper.hpp>
#include <vvipers/debug.hpp>

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