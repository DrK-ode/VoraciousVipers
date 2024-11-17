#pragma once

#include <chrono>

namespace VVipers {

typedef std::chrono::duration<double>
    Time;  // Holds number of nanoseconds as a double

inline const Time time_from_seconds(double t) {
    return std::chrono::duration_cast<Time>(std::chrono::duration<double>(t));
}

template <typename T, typename U>
inline double time_as_seconds(const std::chrono::duration<T, U>& t) {
    return std::chrono::duration_cast<std::chrono::duration<double>>(t).count();
}

class Stopwatch {
  public:
    Stopwatch() : _is_running(false) {}
    std::chrono::high_resolution_clock::duration restart() {
        auto tmp = _is_running
                       ? stop()
                       : std::chrono::high_resolution_clock::duration(0);
        start();
        return tmp;
    }
    void start() {
        if (_is_running)
            throw std::logic_error("Stopwatch is already running.");
        _is_running = true;
        _start_time = _last_split = _clock.now();
    }
    std::chrono::high_resolution_clock::duration split() {
        if (!_is_running)
            throw std::logic_error("Stopwatch is not running.");
        auto tmp = _last_split;
        _last_split = _clock.now();
        return _last_split - tmp;
    }
    std::chrono::high_resolution_clock::duration stop() {
        if (!_is_running)
            throw std::logic_error("Stopwatch is not running.");
        _is_running = false;
        return _clock.now() - _start_time;
    }

  private:
    std::chrono::high_resolution_clock _clock;
    bool _is_running;
    std::chrono::high_resolution_clock::time_point _start_time;
    std::chrono::high_resolution_clock::time_point _last_split;
};

}  // namespace VVipers
