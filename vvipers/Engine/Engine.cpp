#include <memory>
#include <thread>
#include <vector>
#include <vvipers/Engine/Engine.hpp>
#include <vvipers/Scenes/FlashScreen.hpp>
#include <vvipers/Utilities/Time.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

Engine::Engine(const OptionsProvider& options, const FontProvider& fonts,
               const TextureProvider& textures)
    : m_game(options, fonts, textures) {
    loadFirstScene();
}

void Engine::loadFirstScene() {
    m_scenes.push_back(
        std::unique_ptr<Scene>(new FlashScreen(m_game, seconds(2))));
}

void Engine::startGame() {
    double FPS = m_game.getOptionsService().getOptionDouble("General/FPS");
    if (FPS == 0.)
        FPS = 60.0;  // Default

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
    while (!m_scenes.empty()) {
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
                update(nominalFrameDuration);
                tickDuration -= nominalFrameDuration;
            }
            update(tickDuration);
        }
        updateDuration = clock.split();
        m_scenes.back()->processEvents();
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

void Engine::sceneSelection() {
    switch (m_scenes.back()->getTransitionState()) {
        case Scene::TransitionState::Continue: {
            break;
        }
        case Scene::TransitionState::Replace: {
            auto nextScene = m_scenes.back()->getTransition();
            m_scenes.pop_back();
            m_scenes.push_back(std::move(nextScene));
            break;
        }
        case Scene::TransitionState::Return: {
            m_scenes.pop_back();
            break;
        }
        case Scene::TransitionState::Spawn: {
            m_scenes.push_back(std::move(m_scenes.back()->getTransition()));
            break;
        }
        case Scene::TransitionState::Quit: {
            while (!m_scenes.empty()) {
                m_scenes.pop_back();
            }
        }
    }
}

void Engine::update(Time elapsedTime) {
    for (auto& scene : m_scenes)
        if (scene->getSceneState() == Scene::SceneState::Running)
            scene->update(elapsedTime);
}

void Engine::draw() {
    m_scenes.back()->draw();
    m_game.getWindow().display();
}

}  // namespace VVipers
