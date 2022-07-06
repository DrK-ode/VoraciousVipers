#ifndef VVIPERS_TIME_HPP
#define VVIPERS_TIME_HPP

#include <chrono>

namespace VVipers {

//typedef std::chrono::high_resolution_clock::duration Time;
typedef std::chrono::duration<double> Time; // Holds number of nanoseconds as a double

template <typename T, typename U>
Time convertTime(const std::chrono::duration<T, U>& t) {
    return std::chrono::duration_cast<Time>(std::chrono::duration<T, U>(t));
}

inline const Time seconds(double t) {
    return std::chrono::duration_cast<Time>(std::chrono::duration<double>(t));
}

template<typename T,typename U>
inline double toSeconds(const std::chrono::duration<T,U>& t) {
    return std::chrono::duration_cast<std::chrono::duration<double>>(t).count();
}

class StopWatch {
  public:
    StopWatch() : m_isRunning(false) {}
    std::chrono::high_resolution_clock::duration restart() {
        auto tmp = m_isRunning
                       ? stop()
                       : std::chrono::high_resolution_clock::duration(0);
        start();
        return tmp;
    }
    void start() {
        if (m_isRunning)
            throw std::logic_error("StopWatch is already running.");
        m_isRunning = true;
        m_startTime = m_lastSplit = m_clock.now();
    }
    std::chrono::high_resolution_clock::duration split() {
        if (m_isRunning)
            throw std::logic_error("StopWatch is not running.");
        auto tmp = m_lastSplit;
        m_lastSplit = m_clock.now();
        return m_lastSplit - tmp;
    }
    std::chrono::high_resolution_clock::duration stop() {
        if (m_isRunning)
            throw std::logic_error("StopWatch is not running.");
        m_isRunning = false;
        return m_clock.now() - m_startTime;
    }

  private:
    std::chrono::high_resolution_clock m_clock;
    bool m_isRunning;
    std::chrono::high_resolution_clock::time_point m_startTime;
    std::chrono::high_resolution_clock::time_point m_stopTime;
    std::chrono::high_resolution_clock::duration m_prePause;
    std::chrono::high_resolution_clock::time_point m_lastSplit;
};

}  // namespace VVipers

#endif