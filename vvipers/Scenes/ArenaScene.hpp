#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/View.hpp>
#include <memory>
#include <vector>
#include <vvipers/Collisions/CollisionManager.hpp>
#include <vvipers/Engine/GameResources.hpp>
#include <vvipers/Engine/Scene.hpp>
#include <vvipers/GameElements/FlyingScore.hpp>
#include <vvipers/GameElements/Food.hpp>
#include <vvipers/GameElements/GameEvent.hpp>
#include <vvipers/GameElements/GameObject.hpp>
#include <vvipers/GameElements/Observer.hpp>
#include <vvipers/GameElements/Player.hpp>
#include <vvipers/GameElements/Viper.hpp>
#include <vvipers/GameElements/Walls.hpp>
#include <vvipers/UIElements/Controller.hpp>
#include <vvipers/UIElements/PlayerPanel.hpp>
#include <vvipers/Utilities/Time.hpp>

namespace VVipers {

class ArenaScene : public Scene {
  public:
    ArenaScene(GameResources& game);
    ~ArenaScene();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void on_notify(const GameEvent& event) override;

  private:
    struct PlayerData {
        std::string name;
        sf::Color primary_color;
        sf::Color secondary_color;
        std::vector<int> keys;
        bool mouse_enabled;
    };
    void add_food(Vec2 position, double diameter);
    Player* add_player(const PlayerData&, std::unique_ptr<Controller>,
                       std::shared_ptr<Viper>, sf::View view);
    void add_players(std::vector<PlayerData>&, std::vector<sf::View>&);
    std::shared_ptr<Viper> add_viper(
        std::vector<Polygon>& excluded_starting_areas);
    void check_for_game_over();
    void delete_player(Player* player);
    void delete_viper(Viper* viper);
    void delete_food(Food* food);
    void dispense_food();
    PlayerPanel* find_player_panel(const Player* player) const;
    Player* find_player_with(const Viper*) const;
    Player* find_player_with(const Controller*) const;
    void find_free_space_for(Shape& shape, bool allow_rotation = false,
                             const std::vector<Polygon>& exclusion_zones =
                                 std::vector<Polygon>()) const;
    void handle_collision(const CollisionPair&);
    void handle_collisions();
    void handle_destruction(const GameObject* event);
    void handle_steering(const Controller*);
    void handle_viper_food_collision(Viper*, size_t, Food*, size_t);
    void handle_viper_viper_collision(Viper*, size_t, Viper*, size_t);
    void handle_viper_walls_collision(Viper*, size_t, Walls*, size_t);
    void kill_viper(Viper* viper);
    void process_deletions();
    PlayerData read_player_conf(size_t player);
    void update(Time elapsedTime);

    sf::View _game_view;
    // The arena keeps partial ownership of the pause screen in order to be able
    // to reuse it
    std::shared_ptr<Scene> _transition_scene;
    std::shared_ptr<Scene> _pause_scene;
    // Arena and Player has joint ownership of controllers and vipers
    std::vector<std::shared_ptr<Viper>> _vipers;
    std::vector<std::unique_ptr<Player>> _players;
    std::vector<std::unique_ptr<PlayerPanel>> _player_panels;
    std::vector<std::unique_ptr<Food>> _food;
    std::vector<std::unique_ptr<FlyingScore>> _flying_scores;
    std::unique_ptr<Walls> _walls;

    std::set<const GameObject*> _objects_to_delete;
    CollisionManager _collision_manager;
};

}  // namespace VVipers
