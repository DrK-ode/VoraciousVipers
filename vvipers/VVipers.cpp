#include <algorithm>
#include <thread>
#include <vvipers/Time.hpp>
#include <vvipers/VVipers.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

void VVipers::startGame() {
    Time tickDuration(0), updateDuration(0), eventDuration(0), drawDuration(0),
        sleepDuration(0), debtDuration(0), debugDuration(0);
    const Time nominalFrameDuration = seconds(1. / 60);
    Time frameDuration = nominalFrameDuration;
    double fpsAverage = 0.;
    const size_t sampleSize = 60;
    std::vector<double> durationSamples(sampleSize, 0.);
    size_t sampleIndex = 0;

    Stopwatch clock;
    clock.start();
    bool firstFrame = true;
    // Main game loop
    while (!m_game.exit()) {
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
            logInfo("  Debug:   ", debugDuration);
            logInfo("  Update:  ", updateDuration);
            logInfo("  Events:  ", eventDuration);
            logInfo("  Drawing: ", drawDuration);
            logInfo("  Sleep:   ", sleepDuration);
            logInfo("  We owe the next frame: ", debtDuration);
        }
        debugDuration = clock.split();

        if (!firstFrame) {
            while (tickDuration > 2 * nominalFrameDuration) {
                m_game.update(nominalFrameDuration);
                tickDuration -= nominalFrameDuration;
            }
            m_game.update(tickDuration);
        }

        updateDuration = clock.split();

        m_game.processEvents();

        eventDuration = clock.split();

        m_game.draw();
        m_game.display();

        drawDuration = clock.split();
        sleepDuration =
            frameDuration -
            (debugDuration, updateDuration + eventDuration + drawDuration);

        std::this_thread::sleep_for(sleepDuration);
        firstFrame = false;
    }
}

}  // namespace VVipers