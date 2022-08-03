#include <SFML/Window/Mouse.hpp>
#include <vvipers/Scenes/MenuScene.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

MenuScene::MenuScene(Game& game, sf::View view)
    : m_game(game), m_view(view), m_selectedIndex(-1), m_layout(Vertical) {}

void MenuScene::addItem(MenuItem* menuItem) { m_menuItems.push_back(menuItem); }

void MenuScene::delItem(size_t index) {
    m_menuItems.erase(m_menuItems.begin() + index);
    // If the iterator points to the object erased or beyond, it is invalidated
    if (m_selectedIndex >= index)
        --m_selectedIndex;
}

void MenuScene::draw() {
    m_game.getWindow().setView(m_view);
    for (auto& menuItem : m_menuItems)
        m_game.getWindow().draw(*menuItem);
}

void MenuScene::distributeMenuItems() {
    Vec2 size = m_view.getSize();
    Vec2 position;
    if (m_layout == Horizontal) {
        size.x /= m_menuItems.size();
        for (auto& mi : m_menuItems) {
            mi->setPosition(position);
            mi->setSize(size);
            position.x += size.x;
        }
    } else {
        size.y /= m_menuItems.size();
        for (auto& mi : m_menuItems) {
            mi->setPosition(position);
            mi->setSize(size);
            position.y += size.y;
        }
    }
}

void MenuScene::handleKeyPressed(const sf::Event& event) {
    if ((m_layout == Vertical && event.key.code == sf::Keyboard::Up) or
        (m_layout == Horizontal && event.key.code == sf::Keyboard::Left)) {
        auto oldSelectedIndex = m_selectedIndex;
        if (m_selectedIndex == 0)
            m_selectedIndex = m_menuItems.size();
        --m_selectedIndex;
        swapSelected(oldSelectedIndex, m_selectedIndex);
    } else if ((m_layout == Vertical && event.key.code == sf::Keyboard::Down) or
               (m_layout == Horizontal &&
                event.key.code == sf::Keyboard::Right)) {
        auto oldSelectedIndex = m_selectedIndex;
        if (oldSelectedIndex >= m_menuItems.size())
            m_selectedIndex = 0;
        else
            ++m_selectedIndex;
        if (m_selectedIndex == m_menuItems.size())
            m_selectedIndex = 0;
        swapSelected(oldSelectedIndex, m_selectedIndex);
    } else if (event.key.code == sf::Keyboard::Return) {
        if (m_selectedIndex < m_menuItems.size())
            onMenuItemActivation(m_menuItems[m_selectedIndex]);
    } else if (event.key.code == sf::Keyboard::Escape) {
        onReturn();
    } else {
        if (m_selectedIndex < m_menuItems.size())
            m_menuItems[m_selectedIndex]->handleEvent(event);
    }
}

void MenuScene::handleMouseMoved(const sf::Event& event) {
    auto oldSelectedIndex = m_selectedIndex;
    auto& window = m_game.getWindow();
    Vec2 localCoord =
        window.mapPixelToCoords(sf::Mouse::getPosition(window), m_view);
    auto pointedAt = menuItemAtCoords(localCoord);
    for (size_t i = 0; i < m_menuItems.size(); ++i) {
        if (m_menuItems[i] == pointedAt) {
            m_selectedIndex = i;
            break;
        }
    }
    swapSelected(oldSelectedIndex, m_selectedIndex);
}

void MenuScene::handleMouseButtonPressed(const sf::Event& event) {
    auto& window = m_game.getWindow();
    Vec2 localCoord =
        window.mapPixelToCoords(sf::Mouse::getPosition(window), m_view);
    if (m_selectedIndex < m_menuItems.size() and
        m_menuItems[m_selectedIndex] == menuItemAtCoords(localCoord))
        onMenuItemActivation(m_menuItems[m_selectedIndex]);
}

void MenuScene::handleTextEntered(const sf::Event& event) {
    if (m_selectedIndex < m_menuItems.size())
        m_menuItems[m_selectedIndex]->handleEvent(event);
}

void MenuScene::processEvents() {
    sf::Event event;
    while (m_game.getWindow().pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed: {
                setTransitionState(TransitionState::Quit);
                break;
            }
            case sf::Event::TextEntered: {
                handleTextEntered(event);
                break;
            }
            case sf::Event::KeyPressed: {
                handleKeyPressed(event);
                break;
            }
            case sf::Event::MouseButtonPressed: {
                handleMouseButtonPressed(event);
                break;
            }
            case sf::Event::MouseMoved: {
                handleMouseMoved(event);
                break;
            }
            default:
                break;
        }
    }
}

MenuItem* MenuScene::menuItemAtCoords(Vec2 coords) const {
    for (auto menuItem : m_menuItems)
        if (menuItem->contains(coords))
            return menuItem;
    return nullptr;
}

const MenuItem* MenuScene::getSelected() const {
    if (m_selectedIndex < m_menuItems.size())
        return m_menuItems[m_selectedIndex];
    return nullptr;
}

void MenuScene::setSelectedIndex(size_t index) {
    swapSelected(m_selectedIndex, index);
    m_selectedIndex = index;
}

void MenuScene::swapSelected(size_t oldSelectedIndex, size_t newSelectedIndex) {
    if (oldSelectedIndex < m_menuItems.size())
        m_menuItems[oldSelectedIndex]->setSelected(false);
    if (newSelectedIndex < m_menuItems.size())
        m_menuItems[newSelectedIndex]->setSelected(true);
}

/** Default is to update all the menu items **/
void MenuScene::update(Time elapsedTime) { updateMenuItems(elapsedTime); }

void MenuScene::updateMenuItems(Time elapsedTime) {
    for (auto& menuItem : m_menuItems)
        menuItem->update(elapsedTime);
}

void MenuScene::onReactivation() {
    setSceneState(Scene::SceneState::Running);
    setTransitionState(TransitionState::Continue);
    m_selectedIndex = 0;
}

}  // namespace VVipers
