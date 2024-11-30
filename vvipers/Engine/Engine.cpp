#include <SFML/Window/Event.hpp>
#include <cmath>
#include <memory>
#include <stdexcept>
#include <thread>
#include <vector>
#include <vvipers/Engine/Engine.hpp>
#include <vvipers/Utilities/Time.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>

#include "vvipers/GameElements/GameEvent.hpp"

namespace VVipers {

Engine::Engine(std::unique_ptr<GameResources> game_resources)
    : _game_resources(std::move(game_resources)) {}

void Engine::start_game() {
    double FPS =
        _game_resources->options_service().option_double("General/FPS");
    if (FPS == 0.)
        FPS = 60.0;  // Default

    if (_scenes.empty()) {
        if (_defaultScene) {
            add_scene(_defaultScene);
        } else {
            tag_error("No scene loaded to start the game.");
        }
    }

    game_loop(FPS);
}

void Engine::game_loop(double FPS) {
    Time tick_duration(0), update_duration(0), event_duration(0),
        draw_duration(0), sleep_duration(0), debt_duration(0),
        debug_duration(0);
    const Time nominalFrameDuration = time_from_seconds(1. / FPS);
    Time frameDuration = nominalFrameDuration;
    double fpsAverage = 0.;
    const size_t sampleSize = FPS;
    std::vector<double> durationSamples(sampleSize, 0.);
    size_t sampleIndex = 0;

    Stopwatch clock;
    clock.start();
    bool firstFrame = true;
    // Main game loop
    while (!_scenes.empty()) {
        tick_duration = clock.restart();
        // If not first tick
        if (!firstFrame) {
            // Analyze last event
            debt_duration = frameDuration - tick_duration;
            frameDuration = nominalFrameDuration + debt_duration;
            double fps = 1 / time_as_seconds(tick_duration);
            // Calculate average FPS
            durationSamples[sampleIndex++] = fps;
            fpsAverage = 0.;
            for (auto& t : durationSamples)
                fpsAverage += t;
            fpsAverage /= sampleSize;
            if (sampleIndex == sampleSize)
                sampleIndex = 0;
            // Print some info
            log_info("Last frame took ", tick_duration, " (on average ",
                     fpsAverage, " FPS)");
            log_info("  Debug:           ", debug_duration);
            log_info("  Update:          ", update_duration);
            log_info("  Events:          ", event_duration);
            log_info("  Drawing:         ", draw_duration);
            log_info("  Sleep:           ", sleep_duration);
            log_info("  We owe the next frame: ", debt_duration);
        }
        debug_duration = clock.split();
        if (!firstFrame) {
            notify(UpdateEvent(tick_duration));
        }
        update_duration = clock.split();
        process_window_events();
        event_duration = clock.split();
        draw();
        draw_duration = clock.split();
        process_scene_events();

        sleep_duration =
            std::max(time_from_seconds(0),
                     frameDuration - (debug_duration + update_duration +
                                      event_duration + draw_duration));
        std::this_thread::sleep_for(sleep_duration);
        firstFrame = false;
    }
}

void Engine::process_window_events() {
    sf::Event event;
    while (_game_resources->window_manager()->poll_event(event)) {
        switch (event.type) {
            case sf::Event::Closed: {
                _scenes.clear();
                break;
            }
            case sf::Event::Resized: {
                notify(
                    WindowEvent(WindowEvent::WindowEventType::WindowResized));
                break;
            }
            case sf::Event::KeyPressed: {
                notify(
                    KeyboardEvent(KeyboardEvent::KeyboardEventType::KeyPressed,
                                  event.key.scancode));
                break;
            }
            case sf::Event::KeyReleased: {
                notify(
                    KeyboardEvent(KeyboardEvent::KeyboardEventType::KeyReleased,
                                  event.key.scancode));
                break;
            }
            case sf::Event::MouseWheelScrolled: {
                MouseEvent mouse_event(
                    MouseEvent::MouseEventType::Scroll,
                    {event.mouseWheelScroll.x, event.mouseWheelScroll.y});
                mouse_event.scroll = event.mouseWheelScroll.delta;
                notify(mouse_event);
                break;
            }
            case sf::Event::MouseButtonPressed: {
                MouseEvent mouse_event(
                    MouseEvent::MouseEventType::ButtonPressed,
                    {event.mouseButton.x, event.mouseButton.y});
                mouse_event.mouse_button = event.mouseButton.button;
                notify(mouse_event);
                break;
            }
            case sf::Event::MouseButtonReleased: {
                MouseEvent mouse_event(
                    MouseEvent::MouseEventType::ButtonReleased,
                    {event.mouseButton.x, event.mouseButton.y});
                mouse_event.mouse_button = event.mouseButton.button;
                notify(mouse_event);
                break;
            }
            case sf::Event::MouseMoved: {
                MouseEvent mouse_event(MouseEvent::MouseEventType::Move,
                                       {event.mouseMove.x, event.mouseMove.y});
                notify(mouse_event);
                break;
            }
            default:
                break;
        }
    }
}

void Engine::add_scene(const std::shared_ptr<Scene>& scene) {
    scene->add_observer(this, {GameEvent::EventType::Scene});
    add_observer(scene.get(),
                 {GameEvent::EventType::Keyboard, GameEvent::EventType::Mouse,
                  GameEvent::EventType::Update, GameEvent::EventType::Window});
    _scenes.push_back(scene);
    scene->on_activation();
}

void Engine::pop_scene() {
    _scenes.back()->remove_observer(this);
    remove_observer(_scenes.back().get());
    _scenes.pop_back();
}

void Engine::process_scene_events() {
    if (_scene_events.size() == 0) {
        return;
    } else if (_scene_events.size() > 1) {
        throw std::runtime_error("More than one SceneEvent has happened.");
    }
    auto& next_scene = _scene_events[0].target_scene;
    switch (_scene_events[0].scene_event_type) {
        case SceneEvent::SceneEventType::Clear: {
            _scenes.clear();
            add_scene(next_scene);
            break;
        }
        case SceneEvent::SceneEventType::Default: {
            _scenes.clear();
            if (_defaultScene) {
                add_scene(_defaultScene);
                break;
            }
        }
        case SceneEvent::SceneEventType::JumpTo: {
            while (_scenes.back() != next_scene) {
                pop_scene();
            }
            if (!_scenes.empty())
                _scenes.back()->on_activation();
            break;
        }
        case SceneEvent::SceneEventType::Replace: {
            pop_scene();
            add_scene(next_scene);
            break;
        }
        case SceneEvent::SceneEventType::Return: {
            pop_scene();
            if (!_scenes.empty())
                _scenes.back()->on_activation();
            break;
        }
        case SceneEvent::SceneEventType::Spawn: {
            add_scene(next_scene);
            break;
        }
        case SceneEvent::SceneEventType::Quit: {
            _scenes.clear();
        }
    }
    _scene_events.clear();
}

void Engine::on_notify(const GameEvent& event) {
    switch (event.type()) {
        case GameEvent::EventType::Scene: {
            const SceneEvent& scene_event =
                dynamic_cast<const SceneEvent&>(event);
            _scene_events.push_back(scene_event);
        }
        default:
            break;
    }
}

void Engine::draw() {
    auto window_manager = _game_resources->window_manager();
    window_manager->clear(sf::Color::Black);
    auto sceneIter = _scenes.rbegin();
    // Find top-most scene that is solid
    while (std::next(sceneIter) != _scenes.rend() &&
           (*sceneIter)->draw_state() != Scene::DrawState::Solid) {
        ++sceneIter;
    }
    while (sceneIter != _scenes.rbegin()) {
        if ((*sceneIter)->draw_state() != Scene::DrawState::Skip) {
            window_manager->draw(*(sceneIter)->get());
        }
        sceneIter--;
    }
    window_manager->draw(*_scenes.back());
    window_manager->display();
}
}  // namespace VVipers
