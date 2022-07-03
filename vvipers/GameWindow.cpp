#include <vvipers/GameWindow.hpp>

namespace VVipers {

GameWindow::GameWindow()
    : sf::RenderWindow(sf::VideoMode(800, 600), "VoraciousVipers") {}

GameWindow::~GameWindow() {}

}