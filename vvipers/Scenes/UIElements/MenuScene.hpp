#ifndef VVIPERS_SCENES_UIELEMENTS_MENUSCENE_HPP
#define VVIPERS_SCENES_UIELEMENTS_MENUSCENE_HPP

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

    MenuScene(Game& game);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void distributeMenuItems();
    void processEvent(const sf::Event& event) override;
    /** Overide if you need to do anything else but calling update on all menu
     * items. **/
    void update(Time elapsedTime) override;
    // Calls update on all menuItems
    void updateMenuItems(Time elapsedTime);
    sf::View getMenuView() const { return m_menuView; }
    void setMenuView(sf::View view) { m_menuView = view; }
    void addItem(MenuItem* menuItem);
    void delItem(size_t index);
    const MenuItem* getItem(int index) const { return m_menuItems[index]; }
    const MenuItem* getSelected() const;
    void setSelectedIndex(size_t index);
    size_t getNumberOfMenuItems() const { return m_menuItems.size(); }
    LayoutOrientation getLayoutOrientation() const { return m_layout; }
    void setLayoutOrientation(LayoutOrientation lo) { m_layout = lo; }
    virtual void onMenuItemActivation(MenuItem* menuItem){};
    virtual void onReturn() { setTransitionState(TransitionState::Return); }
    virtual void onActivation() override;

  protected:
    virtual void drawBackground(sf::RenderTarget& target,
                        sf::RenderStates states) const {};

  private:
    void handleKeyPressed(const sf::Event& event);
    void handleMouseMoved(const sf::Event& event);
    void handleMouseButtonPressed(const sf::Event& event);
    void handleMouseScroll(const sf::Event& event);
    void handleTextEntered(const sf::Event& event);

    void menuUp();
    void menuDown();
    MenuItem* menuItemAtCoords(Vec2 coords) const;
    void activateSelected();
    void swapSelected(size_t oldSelected, size_t newSelected);

    sf::View m_menuView;
    std::vector<MenuItem*> m_menuItems;
    size_t m_selectedIndex;
    LayoutOrientation m_layout;
};

}  // namespace VVipers

#endif // VVIPERS_SCENES_UIELEMENTS_MENUSCENE_HPP
