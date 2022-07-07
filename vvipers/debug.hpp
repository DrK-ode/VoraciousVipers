#ifndef VVIPERS_DEBUG_HPP
#define VVIPERS_DEBUG_HPP

#include <iostream>
#include <source_location>
#include <string_view>

namespace VVipers {

enum class LogLevel { onlyErrors = 0, errorsAndWarnings = 1, all = 2 };
inline LogLevel logLevel = LogLevel::all;

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
inline void warnTag(
    const std::source_location& location = std::source_location::current()) {
    if (logLevel >= LogLevel::errorsAndWarnings)
        tag(location, std::cerr);
}
inline void errTag(
    const std::source_location& location = std::source_location::current()) {
    if (logLevel >= LogLevel::onlyErrors)
        tag(location, std::cerr);
}
#else
inline void tag(const std::source_location& location = std::source_location(),
                std::ostream& os = std::cout) {}
inline void infoTag(
    const std::source_location& location = std::source_location()) {}
inline void warnTag(
    const std::source_location& location = std::source_location()) {}
inline void errTag(
    const std::source_location& location = std::source_location()) {}
#endif

template <typename... Args>
inline void _implLogInfo(bool doTag, const std::source_location loc,
                         Args&&... args) {
    if (logLevel >= LogLevel::all) {
        if (doTag)
            tag(loc, std::cout);
        std::cout << "   INFO   ";
        (std::cout << ... << std::forward<Args>(args)) << '\n';
    }
}

template <typename... Args>
inline void _implLogWarning(bool doTag, const std::source_location loc,
                            Args&&... args) {
    if (logLevel >= LogLevel::errorsAndWarnings) {
        if (doTag)
            tag(loc, std::cerr);
        std::cerr << "   WARN   ";
        (std::cerr << ... << std::forward<Args>(args)) << '\n';
    }
}

template <typename... Args>
inline void _implLogError(bool doTag, const std::source_location loc,
                          Args&&... args) {
    if (logLevel >= LogLevel::onlyErrors) {
        if (doTag)
            tag(loc, std::cerr);
        std::cerr << "   ERR    ";
        (std::cerr << ... << std::forward<Args>(args)) << '\n';
    }
}

#ifndef __INTELLISENSE__  // Because intellisense bugs on
                          // std::source_location::current() and this removes
                          // the error squiggles
#define logInfo(...) \
    _implLogInfo(false, std::source_location::current(), __VA_ARGS__)
#define logWarning(...)                                              \
    _implLogWarning(false, std::source_location::current(), \
                             __VA_ARGS__)
#define logError(...) \
    _implLogError(false, std::source_location::current(), __VA_ARGS__)
#define tagInfo(...) \
    _implLogInfo(true, std::source_location::current(), __VA_ARGS__)
#define tagWarning(...) \
    _implLogWarning(true, std::source_location::current(), __VA_ARGS__)
#define tagError(...) \
    _implLogError(true, std::source_location::current(), __VA_ARGS__)
#else
#define logInfo(...) \
    _implLogInfo(false, std::source_location(), __VA_ARGS__)
#define logWarn(...) \
    _implLogWarning(false, std::source_location(), __VA_ARGS__)
#define logErr(...) \
    _implLogError(false, std::source_location(), __VA_ARGS__)
#define tagInfo(...) \
    _implLogInfo(true, std::source_location(), __VA_ARGS__)
#define tagWarn(...) \
    _implLogWarning(true, std::source_location(), __VA_ARGS__)
#define tagErr(...) \
    _implLogError(true, std::source_location(), __VA_ARGS__)
#endif

}  // namespace VVipers
#endif