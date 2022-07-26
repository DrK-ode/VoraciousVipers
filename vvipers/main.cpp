//#include <algorithm>
#include <thread>
#include <vvipers/Game.hpp>
#include <vvipers/GameConfiguration.hpp>
#include <vvipers/Time.hpp>
#include <vvipers/config.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

GameConfiguration* readGameConfig(const std::string& cfgFile) {
    return new GameConfiguration(cfgFile);
}

void startGame(const GameConfiguration* cfg) {
    Vec2 windowSize = cfg->getValueAsVec2("windowSize");
    double FPS = cfg->getValueAsDouble("FPS");
    if (FPS == 0.)
        FPS = 60.0;  // Default

    Game theGame(windowSize);

    Time tickDuration(0), updateDuration(0), eventDuration(0), drawDuration(0),
        sleepDuration(0), debtDuration(0), debugDuration(0);
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
    while (!theGame.exit()) {
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
                theGame.update(nominalFrameDuration);
                tickDuration -= nominalFrameDuration;
            }
            theGame.update(tickDuration);
        }
        updateDuration = clock.split();
        theGame.processEvents();
        eventDuration = clock.split();
        theGame.draw();
        theGame.display();
        drawDuration = clock.split();
        sleepDuration =
            frameDuration -
            (debugDuration, updateDuration + eventDuration + drawDuration);

        std::this_thread::sleep_for(sleepDuration);
        firstFrame = false;
    }
}

}  // namespace VVipers

int main(int argc, const char** argv) {
    VVipers::verbosityLevel = VVipers::Verbosity::errorsAndWarnings;
    // Handle input arguments
    auto cfg = VVipers::readGameConfig(USER_CONFIGURATION_FILE_PATH);
    VVipers::startGame(cfg);
    return 0;
}