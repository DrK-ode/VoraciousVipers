#ifndef VVIPERS_SCENES_ARENASCENE_HPP
#define VVIPERS_SCENES_ARENASCENE_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/View.hpp>
#include <memory>
#include <vector>
#include <vvipers/Collisions/CollisionManager.hpp>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Engine/Scene.hpp>
#include <vvipers/GameElements/Food.hpp>
#include <vvipers/GameElements/GameEvent.hpp>
#include <vvipers/GameElements/GameObject.hpp>
#include <vvipers/GameElements/Observer.hpp>
#include <vvipers/GameElements/Player.hpp>
#include <vvipers/GameElements/Viper.hpp>
#include <vvipers/GameElements/Walls.hpp>
#include <vvipers/UIElements/Controller.hpp>
#include <vvipers/UIElements/FlyingScore.hpp>
#include <vvipers/UIElements/PlayerPanel.hpp>
#include <vvipers/Utilities/Time.hpp>

namespace VVipers {

using controller_ptr = std::shared_ptr<Controller>;
using food_ptr = std::unique_ptr<Food>;
using player_ptr = std::shared_ptr<Player>;
using viper_ptr = std::shared_ptr<Viper>;
using walls_ptr = std::unique_ptr<Walls>;

class ArenaScene : public Scene, public Observer {
  public:
    ArenaScene(Game& game);
    ~ArenaScene();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void onNotify(const GameEvent* event) override;
    void processEvent(const sf::Event& event) override;
    void update(Time elapsedTime) override;
    std::shared_ptr<Scene> makeTransition() override;

  private:
    controller_ptr createController(
        const KeyboardController::KeyboardControls& keys);
    player_ptr addPlayer(const std::string& name, sf::Color primaryColor,
                         sf::Color secondaryColor, controller_ptr controller,
                         viper_ptr viper, sf::View view);
    void addPlayers(std::vector<std::string>& names,
                    std::vector<std::string>& primaryColors,
                    std::vector<std::string>& secondaryColors,
                    std::vector<double>& keys, std::vector<sf::View>& views);
    void deletePlayer(player_ptr player);
    void addVipers(size_t);
    void deleteViper(Viper* viper);
    void killViper(Viper* viper);
    void addFood(Vec2 position, double diameter);
    void eatFood(Viper*, Food*);
    void deleteFood(Food* food);

    PlayerPanel* findPlayerPanel(const Player* player) const;
    Player* findPlayerWith(const Viper*) const;
    Player* findPlayerWith(const Controller*) const;

    Vec2 find_free_circular_space(double radius);
    std::pair<Vec2,double> find_free_rectangular_space(double width, double height, bool allow_rotation);

    void dispenseFood();

    void processGameEvents();
    void handleCollision(const CollisionPair&);
    void handleViperCollision(const CollisionPair&);
    void handleCollisions();
    void handleSteering();
    void handleDestruction(const DestroyEvent* event);
    void handleObjectUpdates(Time elapsedTime);
    void checkForGameOver();

    void processWindowEvents();

    sf::View _gameView;
    // The arena keeps partial ownership of the pause screen in order to be able
    // to reuse it
    std::shared_ptr<Scene> _transitionScene;
    std::shared_ptr<Scene> _pauseScene;
    // Arena and Player has joint ownership of controllers and vipers
    std::vector<controller_ptr> _controllers;
    std::vector<viper_ptr> _vipers;
    std::vector<player_ptr> _players;
    std::vector<std::unique_ptr<PlayerPanel>> _player_panels;
    std::vector<food_ptr> _food;
    std::vector<std::unique_ptr<FlyingScore>> _flying_scores;
    walls_ptr _walls;

    std::multimap<GameEvent::EventType, const GameEvent*>
        _events_to_be_processed;
    CollisionManager _collision_manager;
};

}  // namespace VVipers

#endif  // VVIPERS_SCENES_ARENASCENE_HPP
