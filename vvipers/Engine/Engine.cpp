#include <memory>
#include <thread>
#include <vector>
#include <vvipers/Engine/Engine.hpp>
#include <vvipers/Utilities/Time.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace VVipers {

Engine::Engine(std::unique_ptr<Game> game) : m_game(std::move(game)) {}

void Engine::startGame() {
    double FPS = m_game->getOptionsService().getOptionDouble("General/FPS");
    if (FPS == 0.)
        FPS = 60.0;  // Default

    if (getSceneStack().empty()) {
        if (m_defaultScene) {
            getSceneStack().push_back(m_defaultScene);
        } else {
            tagError("No scene loaded to start the game.");
        }
    }

    gameLoop(FPS);
}

void Engine::gameLoop(double FPS) {
    Time tickDuration(0), updateDuration(0), eventDuration(0), drawDuration(0),
        sceneSelectionDuration(0), sleepDuration(0), debtDuration(0),
        debugDuration(0);
    const Time nominalFrameDuration = seconds(1. / FPS);
    Time frameDuration = nominalFrameDuration;
    double fpsAverage = 0.;
    const size_t sampleSize = FPS;
    std::vector<double> durationSamples(sampleSize, 0.);
    size_t sampleIndex = 0;

    Stopwatch clock;
    clock.start();
    bool firstFrame = true;
    // Main game loop
    while (!getSceneStack().empty()) {
        tickDuration = clock.restart();
        // If not first tick
        if (!firstFrame) {
            // Analyze last event
            debtDuration = frameDuration - tickDuration;
            frameDuration = nominalFrameDuration + debtDuration;
            double fps = 1 / toSeconds(tickDuration);
            // Calculate average FPS
            durationSamples[sampleIndex++] = fps;
            fpsAverage = 0.;
            for (auto& t : durationSamples)
                fpsAverage += t;
            fpsAverage /= sampleSize;
            if (sampleIndex == sampleSize)
                sampleIndex = 0;
            // Print some info
            logInfo("Last frame took ", tickDuration, " (on average ",
                    fpsAverage, " FPS)");
            logInfo("  Debug:           ", debugDuration);
            logInfo("  Update:          ", updateDuration);
            logInfo("  Events:          ", eventDuration);
            logInfo("  Drawing:         ", drawDuration);
            logInfo("  Scene selection: ", sceneSelectionDuration);
            logInfo("  Sleep:           ", sleepDuration);
            logInfo("  We owe the next frame: ", debtDuration);
        }
        debugDuration = clock.split();
        if (!firstFrame) {
            while (tickDuration > 2 * nominalFrameDuration) {
                // update(nominalFrameDuration);
                tickDuration -= nominalFrameDuration;
            }
            update(tickDuration);
        }
        updateDuration = clock.split();
        processEvents(getSceneStack().back().get());
        eventDuration = clock.split();
        draw();
        drawDuration = clock.split();

        // Check Scene status
        sceneSelection();
        sceneSelectionDuration = clock.split();

        sleepDuration = frameDuration - (debugDuration + updateDuration +
                                         eventDuration + drawDuration);
        std::this_thread::sleep_for(sleepDuration);
        firstFrame = false;
    }
}

void Engine::processEvents(Scene* scene){
    sf::Event event;
    while( getWindow().pollEvent(event)){
        scene->processEvent(event);
    }
}

void Engine::sceneSelection() {
    switch (getSceneStack().back()->getTransitionState()) {
        case Scene::TransitionState::Clear: {
            auto nextScene = getSceneStack().back()->makeTransition();
            getSceneStack().clear();
            getSceneStack().push_back(std::move(nextScene));
            break;
        }
        case Scene::TransitionState::Continue: {
            break;
        }
        case Scene::TransitionState::Default: {
            getSceneStack().clear();
            if (m_defaultScene) {
                getSceneStack().push_back(m_defaultScene);
                m_defaultScene->onReactivation();
                break;
            }
            case Scene::TransitionState::JumpTo: {
                auto nextScene = getSceneStack().back()->makeTransition();
                while (getSceneStack().back() != nextScene)
                    getSceneStack().pop_back();
                if (!getSceneStack().empty())
                    getSceneStack().back()->onReactivation();
                break;
            }
            case Scene::TransitionState::Replace: {
                auto nextScene = getSceneStack().back()->makeTransition();
                getSceneStack().pop_back();
                getSceneStack().push_back(std::move(nextScene));
                break;
            }
            case Scene::TransitionState::Return: {
                getSceneStack()
                    .back()
                    ->makeTransition();  // Ignore return value
                getSceneStack().pop_back();
                if (!getSceneStack().empty())
                    getSceneStack().back()->onReactivation();
                break;
            }
            case Scene::TransitionState::Spawn: {
                getSceneStack().push_back(
                    std::move(getSceneStack().back()->makeTransition()));
                break;
            }
            case Scene::TransitionState::Quit: {
                getSceneStack()
                    .back()
                    ->makeTransition();  // Ignore return value
                getSceneStack().clear();
            }
        }
    }
}

void Engine::update(Time elapsedTime) {
    for (auto& scene : getSceneStack())
        if (scene->getSceneState() == Scene::SceneState::Running)
            scene->update(elapsedTime);
}

void Engine::draw() {
    auto& window = m_game->m_window;
    window.clear(sf::Color::Black);
    auto sceneIter = getSceneStack().rbegin();
    while ((*sceneIter)->isTransparent() &&
           std::next(sceneIter) != getSceneStack().rend())
        ++sceneIter;
    while (sceneIter != getSceneStack().rbegin()) {
        window.draw(*(sceneIter--)->get());
    }
    window.draw(*getSceneStack().back());
    window.display();
}

}  // namespace VVipers
