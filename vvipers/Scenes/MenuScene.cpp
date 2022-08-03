#include <SFML/Window/Mouse.hpp>
#include <vvipers/Scenes/MenuScene.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

MenuScene::MenuScene(Game& game, sf::View view)
    : m_game(game),
      m_view(view),
      m_selected(m_menuItems.end()),
      m_layout(Vertical) {}

void MenuScene::addItem(MenuItem* menuItem) {
    m_menuItems.push_back(menuItem);
    // If the vector reallocates the iterator is invalidated
    m_selected = m_menuItems.end();
}

void MenuScene::delItem(int index) {
    m_menuItems.erase(m_menuItems.begin() + index);
    // If the iterator points to the object erased or beyond, it is invalidated
    m_selected = m_menuItems.end();
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

const MenuItem* MenuScene::getSelected() const {
    if (m_selected == m_menuItems.end())
        return nullptr;
    return *m_selected;
}

void MenuScene::handleKeyPressed(const sf::Event& event) {
    if ((m_layout == Vertical && event.key.code == sf::Keyboard::Up) or
        (m_layout == Horizontal && event.key.code == sf::Keyboard::Left)) {
        auto oldSelected = m_selected;
        if (m_selected == m_menuItems.begin())
            m_selected = m_menuItems.end();
        --m_selected;
        swapSelected(oldSelected, m_selected);
    } else if ((m_layout == Vertical && event.key.code == sf::Keyboard::Down) or
               (m_layout == Horizontal &&
                event.key.code == sf::Keyboard::Right)) {
        auto oldSelected = m_selected;
        if (oldSelected == m_menuItems.end())
            m_selected = m_menuItems.begin();
        else
            ++m_selected;
        if (m_selected == m_menuItems.end())
            m_selected = m_menuItems.begin();
        swapSelected(oldSelected, m_selected);
    } else if (event.key.code == sf::Keyboard::Return) {
        if (m_selected != m_menuItems.end())
            onMenuItemActivation(*m_selected);
    } else if (event.key.code == sf::Keyboard::Escape) {
        onReturn();
    } else {
        if (m_selected != m_menuItems.end())
            (*m_selected)->handleEvent(event);
    }
}

void MenuScene::handleMouseMoved(const sf::Event& event) {
    auto oldSelected = m_selected;
    auto& window = m_game.getWindow();
    Vec2 localCoord =
        window.mapPixelToCoords(sf::Mouse::getPosition(window), m_view);
    tagDebug(localCoord);
    auto pointedAt = menuItemAtCoords(localCoord);
    tagDebug( pointedAt, " ", m_menuItems[0], " ", m_menuItems[1]);
    for (auto menuItemIter = m_menuItems.begin();
         menuItemIter != m_menuItems.end(); ++menuItemIter) {
        if (*menuItemIter == pointedAt) {
            m_selected = menuItemIter;
            break;
        }
    }
    swapSelected(oldSelected, m_selected);
}

void MenuScene::handleMouseButtonPressed(const sf::Event& event) {
    auto& window = m_game.getWindow();
    Vec2 localCoord =
        window.mapPixelToCoords(sf::Mouse::getPosition(window), m_view);
    if (m_selected != m_menuItems.end() and
        *m_selected == menuItemAtCoords(localCoord))
        onMenuItemActivation(*m_selected);
}

void MenuScene::handleTextEntered(const sf::Event& event) {
    if (m_selected != m_menuItems.end())
        (*m_selected)->handleEvent(event);
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
    for (auto menuItem : m_menuItems){
        auto lb = menuItem->getLocalBounds();
        tagDebug( coords, " ", lb.left, " ", lb.top, " ", lb.width, " ", lb.height);
        if (menuItem->contains(coords))
            return menuItem;
    }
    return nullptr;
}

void MenuScene::swapSelected(menuItemContainer::iterator oldSelected,
                             menuItemContainer::iterator newSelected) {
    if (oldSelected != m_menuItems.end())
        (*oldSelected)->setSelected(false);
    if (newSelected != m_menuItems.end())
        (*newSelected)->setSelected(true);
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
    m_selected = m_menuItems.end();
}

}  // namespace VVipers
