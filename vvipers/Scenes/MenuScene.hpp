#ifndef VVIPERS_SCENES_MENUSCENE_HPP
#define VVIPERS_SCENES_MENUSCENE_HPP

#include <SFML/Graphics/View.hpp>
#include <memory>
#include <vector>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Engine/Scene.hpp>
#include <vvipers/Scenes/UIElements/MenuItem.hpp>
#include <vvipers/Utilities/Time.hpp>

namespace VVipers {

class Game;

class MenuScene : public Scene {
  public:
    enum LayoutOrientation { Horizontal, Vertical };

    MenuScene(Game& game, sf::View view);
    void draw() override;
    void distributeMenuItems();
    void processEvents() override;
    /** Overide if you need to do anything else but calling update on all menu
     * items. **/
    void update(Time elapsedTime) override;
    // Calls update on all menuItems
    void updateMenuItems(Time elapsedTime);
    sf::View getView() const { return m_view; }
    void setView(sf::View view) { m_view = view; }
    void addItem(MenuItem* menuItem);
    void delItem(size_t index);
    const MenuItem* getItem(int index) const { return m_menuItems[index]; }
    const MenuItem* getSelected() const;
    void setSelectedIndex( size_t index);
    size_t getNumberOfMenuItems() const { return m_menuItems.size(); }
    LayoutOrientation getLayoutOrientation() const {return m_layout;}
    void setLayoutOrientation( LayoutOrientation lo) {m_layout = lo;}
    virtual void onMenuItemActivation(MenuItem* menuItem){};
    virtual void onReturn() { setTransitionState(TransitionState::Return); }
    virtual void onReactivation() override;

  private:
    void handleKeyPressed(const sf::Event& event);
    void handleMouseMoved(const sf::Event& event);
    void handleMouseButtonPressed(const sf::Event& event);
    void handleTextEntered(const sf::Event& event);

    MenuItem* menuItemAtCoords(Vec2 coords) const;
    void swapSelected(size_t oldSelected,
                      size_t newSelected);

    Game& m_game;
    sf::View m_view;
    std::vector<MenuItem*> m_menuItems;
    size_t m_selectedIndex;
    LayoutOrientation m_layout;
};

}  // namespace VVipers

#endif  // VVIPERS_SCENES_MENUSCENE_HPP
