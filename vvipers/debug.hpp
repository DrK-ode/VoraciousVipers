#ifndef VVIPERS_DEBUG_HPP
#define VVIPERS_DEBUG_HPP

#include <iostream>
#include <source_location>
#include <string_view>

namespace VVipers {

enum class Verbosity { silent = 0, onlyErrors, errorsAndWarnings, all };
inline Verbosity verbosityLevel = Verbosity::all;

#ifndef __INTELLISENSE__  // Because intellisense bugs on
                          // std::source_location::current() and this removes
                          // the error squiggles
inline void tag(const std::source_location& location, std::ostream& os) {
    std::string_view filename(location.file_name());
    os << '@' << filename.substr(filename.find_last_of("/\\") + 1) << ":"
       << location.line() << " " << location.function_name() << std::endl;
}
inline void infoTag(
    const std::source_location& location = std::source_location::current()) {
    if (logLevel >= LogLevel::all)
        tag(location, std::cout);
}
inline void warningTag(
    const std::source_location& location = std::source_location::current()) {
    if (logLevel >= LogLevel::errorsAndWarnings)
        tag(location, std::cerr);
}
inline void errorTag(
    const std::source_location& location = std::source_location::current()) {
    if (logLevel >= LogLevel::onlyErrors)
        tag(location, std::cerr);
}

inline void debugTag(
    const std::source_location& location = std::source_location::current()) {
    tag(location, std::cerr);
}
#else
inline void tag(const std::source_location& location = std::source_location(),
                std::ostream& os = std::cout) {}
inline void infoTag(
    const std::source_location& location = std::source_location()) {}
inline void warningTag(
    const std::source_location& location = std::source_location()) {}
inline void errorTag(
    const std::source_location& location = std::source_location()) {}
inline void debugTag(
    const std::source_location& location = std::source_location()) {}
#endif

template <typename... Args>
inline void _implLogInfo(bool doTag, const std::source_location loc,
                         Args&&... args) {
    if (verbosityLevel >= Verbosity::all) {
        if (doTag)
            tag(loc, std::cout);
        std::cout << "   INFO   ";
        (std::cout << ... << std::forward<Args>(args)) << '\n';
    }
}

template <typename... Args>
inline void _implLogWarning(bool doTag, const std::source_location loc,
                            Args&&... args) {
    if (verbosityLevel >= Verbosity::errorsAndWarnings) {
        if (doTag)
            tag(loc, std::cerr);
        std::cerr << "   WARN   ";
        (std::cerr << ... << std::forward<Args>(args)) << '\n';
    }
}

template <typename... Args>
inline void _implLogError(bool doTag, const std::source_location loc,
                          Args&&... args) {
    if (verbosityLevel >= Verbosity::onlyErrors) {
        if (doTag)
            tag(loc, std::cerr);
        std::cerr << "   ERR    ";
        (std::cerr << ... << std::forward<Args>(args)) << '\n';
    }
}

template <typename... Args>
inline void _implLogDebug(bool doTag, const std::source_location loc,
                          Args&&... args) {
    if (doTag)
        tag(loc, std::cerr);
    std::cerr << "   DEBUG  ";
    (std::cerr << ... << std::forward<Args>(args)) << '\n';
}

#ifndef __INTELLISENSE__  // Because intellisense bugs on
                          // std::source_location::current() and this removes
                          // the error squiggles
#define logInfo(...) \
    _implLogInfo(false, std::source_location::current(), __VA_ARGS__)
#define logWarning(...) \
    _implLogWarning(false, std::source_location::current(), __VA_ARGS__)
#define logError(...) \
    _implLogError(false, std::source_location::current(), __VA_ARGS__)
#define logDebug(...) \
    _implLogDebug(false, std::source_location::current(), __VA_ARGS__)
#define tagInfo(...) \
    _implLogInfo(true, std::source_location::current(), __VA_ARGS__)
#define tagWarning(...) \
    _implLogWarning(true, std::source_location::current(), __VA_ARGS__)
#define tagError(...) \
    _implLogError(true, std::source_location::current(), __VA_ARGS__)
#define tagDebug(...) \
    _implLogDebug(true, std::source_location::current(), __VA_ARGS__)
#else
#define logInfo(...) _implLogInfo(false, std::source_location(), __VA_ARGS__)
#define logWarning(...) \
    _implLogWarning(false, std::source_location(), __VA_ARGS__)
#define logError(...) _implLogError(false, std::source_location(), __VA_ARGS__)
#define logDebug(...) _implLogDebug(false, std::source_location(), __VA_ARGS__)
#define tagInfo(...) _implLogInfo(true, std::source_location(), __VA_ARGS__)
#define tagWarning(...) \
    _implLogWarning(true, std::source_location(), __VA_ARGS__)
#define tagError(...) _implLogError(true, std::source_location(), __VA_ARGS__)
#define tagDebug(...) _implLogDebug(true, std::source_location(), __VA_ARGS__)
#endif

}  // namespace VVipers
#endif