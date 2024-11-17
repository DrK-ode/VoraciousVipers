#ifndef VVIPERS_UIELEMENTS_MENUSCENE_HPP
#define VVIPERS_UIELEMENTS_MENUSCENE_HPP

#include <SFML/Graphics/View.hpp>
#include <vector>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Engine/Scene.hpp>
#include <vvipers/UIElements/MenuItem.hpp>
#include <vvipers/Utilities/Time.hpp>

namespace VVipers {

class Game;

class MenuScene : public Scene {
  public:
    enum LayoutOrientation { Horizontal, Vertical };

    MenuScene(Game& game);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void distributeMenuItems();
    void setColors( sf::Color fill, sf::Color border, sf::Color text);
    void process_event(const sf::Event& event) override;
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
    virtual void on_menu_item_activation(MenuItem* menuItem){};
    virtual void onReturn() { set_transition_state(TransitionState::Return); }
    virtual void on_activation() override;

  protected:
    virtual void draw_background(sf::RenderTarget& target,
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

#endif // VVIPERS_UIELEMENTS_MENUSCENE_HPP
