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

class ArenaScene : public Scene, public Observer {
  public:
    ArenaScene(Game& game);
    ~ArenaScene();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void onNotify(const GameEvent* event) override;
    void processEvent(const sf::Event& event) override;
    void update(Time elapsedTime) override;
    std::shared_ptr<Scene> makeTransition() override {return _transition_scene;}

  private:
    std::unique_ptr<Controller> create_controller(
        const KeyboardController::KeyboardControls& keys);
    Player* add_player(const std::string& name, sf::Color primaryColor,
                      sf::Color secondaryColor,
                      std::unique_ptr<Controller> controller, Viper* viper,
                      sf::View view);
    void add_players(std::vector<std::string>& names,
                    std::vector<std::string>& primaryColors,
                    std::vector<std::string>& secondaryColors,
                    std::vector<double>& keys, std::vector<sf::View>& views);
    void delete_player(Player* player);
    void add_vipers(size_t);
    void delete_viper(Viper* viper);
    void kill_viper(Viper* viper);
    void add_food(Vec2 position, double diameter);
    void eat_food(Viper*, Food*);
    void delete_food(Food* food);

    PlayerPanel* find_player_panel(const Player* player) const;
    Player* find_player_with_viper(const Viper*) const;
    Player* findPlayerWith(const Controller*) const;

    Vec2 find_free_circular_space(double radius);
    std::pair<Vec2, double> find_free_rectangular_space(double width,
                                                        double height,
                                                        bool allow_rotation);

    void dispense_food();

    void process_game_events();
    void handle_collision(const CollisionPair&);
    void handle_viper_collision(const CollisionPair&);
    void handle_collisions();
    void handle_steering();
    void handle_destruction(const DestroyEvent* event);
    void handle_object_updates(Time elapsedTime);
    void check_for_game_over();

    void process_window_events();

    sf::View _game_view;
    // The arena keeps partial ownership of the pause screen in order to be able
    // to reuse it
    std::shared_ptr<Scene> _transition_scene;
    std::shared_ptr<Scene> _pause_scene;
    // Arena and Player has joint ownership of controllers and vipers
    std::vector<std::unique_ptr<Controller>> _controllers;
    std::vector<std::unique_ptr<Viper>> _vipers;
    std::vector<std::unique_ptr<Player>> _players;
    std::vector<std::unique_ptr<PlayerPanel>> _player_panels;
    std::vector<std::unique_ptr<Food>> _food;
    std::vector<std::unique_ptr<FlyingScore>> _flying_scores;
    std::unique_ptr<Walls> _walls;

    std::multimap<GameEvent::EventType, const GameEvent*>
        _events_to_be_processed;
    CollisionManager _collision_manager;
};

}  // namespace VVipers

#endif  // VVIPERS_SCENES_ARENASCENE_HPP
