#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <thread>
#include <vector>
#include <vvipers/Engine/Engine.hpp>
#include <vvipers/Utilities/Time.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>

#include "vvipers/Engine/Providers.hpp"

namespace VVipers {

Engine::Engine(std::unique_ptr<OptionsProvider> options) {
    _game = std::make_unique<Game>(std::move(options), this);
    Vec2 windowSize =
        _game->options_service().option_2d_vector("General/windowSize");
    _window.create(sf::VideoMode(windowSize.x, windowSize.y),
                   "VoraciousVipers");
}

void Engine::start_game() {
    double FPS = _game->options_service().option_double("General/FPS");
    if (FPS == 0.)
        FPS = 60.0;  // Default

    if (_scenes.empty()) {
        if (_defaultScene) {
            _defaultScene->on_activation();
            _scenes.push_back(_defaultScene);
        } else {
            tag_error("No scene loaded to start the game.");
        }
    }

    game_loop(FPS);
}

void Engine::game_loop(double FPS) {
    Time tickDuration(0), updateDuration(0), eventDuration(0), drawDuration(0),
        sceneSelectionDuration(0), sleepDuration(0), debtDuration(0),
        debugDuration(0);
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
        tickDuration = clock.restart();
        // If not first tick
        if (!firstFrame) {
            // Analyze last event
            debtDuration = frameDuration - tickDuration;
            frameDuration = nominalFrameDuration + debtDuration;
            double fps = 1 / time_as_seconds(tickDuration);
            // Calculate average FPS
            durationSamples[sampleIndex++] = fps;
            fpsAverage = 0.;
            for (auto& t : durationSamples)
                fpsAverage += t;
            fpsAverage /= sampleSize;
            if (sampleIndex == sampleSize)
                sampleIndex = 0;
            // Print some info
            log_info("Last frame took ", tickDuration, " (on average ",
                     fpsAverage, " FPS)");
            log_info("  Debug:           ", debugDuration);
            log_info("  Update:          ", updateDuration);
            log_info("  Events:          ", eventDuration);
            log_info("  Drawing:         ", drawDuration);
            log_info("  Scene selection: ", sceneSelectionDuration);
            log_info("  Sleep:           ", sleepDuration);
            log_info("  We owe the next frame: ", debtDuration);
        }
        debugDuration = clock.split();
        if (!firstFrame) {
            update(tickDuration);
        }
        updateDuration = clock.split();
        process_events(_scenes.back().get());
        eventDuration = clock.split();
        draw();
        drawDuration = clock.split();

        // Check Scene status
        scene_selection();
        sceneSelectionDuration = clock.split();

        sleepDuration =
            std::max(time_from_seconds(0),
                     frameDuration - (debugDuration + updateDuration +
                                      eventDuration + drawDuration));
        std::this_thread::sleep_for(sleepDuration);
        firstFrame = false;
    }
}

void Engine::process_events(Scene* scene) {
    sf::Event event;
    while (_window.pollEvent(event)) {
        scene->process_event(event);
    }
}

void Engine::scene_selection() {
    switch (_scenes.back()->transition_state()) {
        case Scene::TransitionState::Clear: {
            auto nextScene = _scenes.back()->make_transition();
            _scenes.clear();
            nextScene->on_activation();
            _scenes.push_back(std::move(nextScene));
            break;
        }
        case Scene::TransitionState::Continue: {
            break;
        }
        case Scene::TransitionState::Default: {
            _scenes.clear();
            if (_defaultScene) {
                _scenes.push_back(_defaultScene);
                _defaultScene->on_activation();
                break;
            }
            case Scene::TransitionState::JumpTo: {
                auto nextScene = _scenes.back()->make_transition();
                while (_scenes.back() != nextScene)
                    _scenes.pop_back();
                if (!_scenes.empty())
                    _scenes.back()->on_activation();
                break;
            }
            case Scene::TransitionState::Replace: {
                auto nextScene = _scenes.back()->make_transition();
                _scenes.pop_back();
                nextScene->on_activation();
                _scenes.push_back(std::move(nextScene));
                break;
            }
            case Scene::TransitionState::Return: {
                _scenes.back()->make_transition();  // Ignore return value
                _scenes.pop_back();
                if (!_scenes.empty())
                    _scenes.back()->on_activation();
                break;
            }
            case Scene::TransitionState::Spawn: {
                auto nextScene = _scenes.back()->make_transition();
                nextScene->on_activation();
                _scenes.push_back(std::move(nextScene));
                break;
            }
            case Scene::TransitionState::Quit: {
                _scenes.back()->make_transition();  // Ignore return value
                _scenes.clear();
            }
        }
    }
}

void Engine::update(Time elapsedTime) {
    for (auto& scene : _scenes)
        if (scene->run_state() == Scene::RunState::Running)
            scene->update(elapsedTime);
}

void Engine::draw() {
    auto& window = _window;
    window.clear(sf::Color::Black);
    auto sceneIter = _scenes.rbegin();
    // Find top-most scene that is solid
    while (std::next(sceneIter) != _scenes.rend() &&
           (*sceneIter)->draw_state() != Scene::DrawState::Solid) {
        ++sceneIter;
    }
    while (sceneIter != _scenes.rbegin()) {
        if ((*sceneIter)->draw_state() != Scene::DrawState::Skip) {
            window.draw(*(sceneIter)->get());
        }
        sceneIter--;
    }
    window.draw(*_scenes.back());
    window.display();
}

void Engine::set_grab_mouse(bool grabbed) {
    _is_mouse_grabbed = grabbed;
    _window.setMouseCursorGrabbed(grabbed);
    _window.setMouseCursorVisible(!grabbed);
}

}  // namespace VVipers
