#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <thread>
#include <vector>
#include <vvipers/Engine/Engine.hpp>
#include <vvipers/Utilities/Time.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

Engine::Engine(std::unique_ptr<Game> game) : _game(std::move(game)) {}

void Engine::start_game() {
    double FPS = _game->options_service().option_double("General/FPS");
    if (FPS == 0.)
        FPS = 60.0;  // Default

    if (scene_stack().empty()) {
        if (_defaultScene) {
            _defaultScene->on_activation();
            scene_stack().push_back(_defaultScene);
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
    while (!scene_stack().empty()) {
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
        process_events(scene_stack().back().get());
        eventDuration = clock.split();
        draw();
        drawDuration = clock.split();

        // Check Scene status
        scene_selection();
        sceneSelectionDuration = clock.split();

        sleepDuration = std::max( time_from_seconds(0), frameDuration - (debugDuration + updateDuration +
                                         eventDuration + drawDuration) );
        std::this_thread::sleep_for(sleepDuration);
        firstFrame = false;
    }
}

void Engine::process_events(Scene* scene) {
    sf::Event event;
    while (window().pollEvent(event)) {
        scene->process_event(event);
    }
}

void Engine::scene_selection() {
    switch (scene_stack().back()->transition_state()) {
        case Scene::TransitionState::Clear: {
            auto nextScene = scene_stack().back()->make_transition();
            scene_stack().clear();
            nextScene->on_activation();
            scene_stack().push_back(std::move(nextScene));
            break;
        }
        case Scene::TransitionState::Continue: {
            break;
        }
        case Scene::TransitionState::Default: {
            scene_stack().clear();
            if (_defaultScene) {
                scene_stack().push_back(_defaultScene);
                _defaultScene->on_activation();
                break;
            }
            case Scene::TransitionState::JumpTo: {
                auto nextScene = scene_stack().back()->make_transition();
                while (scene_stack().back() != nextScene)
                    scene_stack().pop_back();
                if (!scene_stack().empty())
                    scene_stack().back()->on_activation();
                break;
            }
            case Scene::TransitionState::Replace: {
                auto nextScene = scene_stack().back()->make_transition();
                scene_stack().pop_back();
                nextScene->on_activation();
                scene_stack().push_back(std::move(nextScene));
                break;
            }
            case Scene::TransitionState::Return: {
                scene_stack()
                    .back()
                    ->make_transition();  // Ignore return value
                scene_stack().pop_back();
                if (!scene_stack().empty())
                    scene_stack().back()->on_activation();
                break;
            }
            case Scene::TransitionState::Spawn: {
                auto nextScene = scene_stack().back()->make_transition();
                nextScene->on_activation();
                scene_stack().push_back(std::move(nextScene));
                break;
            }
            case Scene::TransitionState::Quit: {
                scene_stack()
                    .back()
                    ->make_transition();  // Ignore return value
                scene_stack().clear();
            }
        }
    }
}

void Engine::update(Time elapsedTime) {
    for (auto& scene : scene_stack())
        if (scene->scene_state() == Scene::SceneState::Running)
            scene->update(elapsedTime);
}

void Engine::draw() {
    auto& window = _game->_window;
    window.clear(sf::Color::Black);
    auto sceneIter = scene_stack().rbegin();
    while ((*sceneIter)->is_transparent() &&
           std::next(sceneIter) != scene_stack().rend())
        ++sceneIter;
    while (sceneIter != scene_stack().rbegin()) {
        window.draw(*(sceneIter--)->get());
    }
    window.draw(*scene_stack().back());
    window.display();
}

}  // namespace VVipers
