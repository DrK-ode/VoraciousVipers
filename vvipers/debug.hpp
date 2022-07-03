#ifndef VVIPERS_DEBUG_HPP
#define VVIPERS_DEBUG_HPP

#include <iostream>
#include <source_location>
#include <string_view>

namespace VVipers {

enum LogLevel { onlyErrors = 0, errorsAndWarnings = 1, all = 2 };
inline LogLevel logLevel = all;

#ifndef __INTELLISENSE__  // Because intellisense bugs on
                          // std::source_location::current() and this removes
                          // the error squiggles
inline void tag(
    const std::source_location& location = std::source_location::current()) {
    std::string_view filename(location.file_name());
    std::cout << '@' << filename.substr(filename.find_last_of("/\\") + 1) << ":"
              << location.line() << " " << location.function_name()
              << std::endl;
}
#else
inline void tag(const std::source_location& location = std::source_location()) {
}
#endif

template <typename... Args>
inline void _implLogInfo(bool doTag, const std::source_location loc,
                         Args&&... args) {
    if (logLevel >= all) {
        if (doTag)
            tag(loc);
        std::cout << "   INFO   ";
        (std::cout << ... << std::forward<Args>(args)) << '\n';
    }
}

template <typename... Args>
inline void _implLogWarning(bool doTag, const std::source_location loc,
                            Args&&... args) {
    if (logLevel >= errorsAndWarnings) {
        if (doTag)
            tag(loc);
        std::cout << "   WARN   ";
        (std::cerr << ... << std::forward<Args>(args)) << '\n';
    }
}

template <typename... Args>
inline void _implLogError(bool doTag, const std::source_location loc,
                          Args&&... args) {
    if (logLevel >= onlyErrors) {
        if (doTag)
            tag(loc);
        std::cout << "   ERR    ";
        (std::cerr << ... << std::forward<Args>(args)) << '\n';
    }
}

#ifndef __INTELLISENSE__  // Because intellisense bugs on
                          // std::source_location::current() and this removes
                          // the error squiggles
#define logInfo(...) \
    VVipers::_implLogInfo(false, std::source_location::current(), __VA_ARGS__)
#define logWarning(...)                                              \
    VVipers::_implLogWarning(false, std::source_location::current(), \
                             __VA_ARGS__)
#define logError(...) \
    VVipers::_implLogError(false, std::source_location::current(), __VA_ARGS__)
#define tagInfo(...) \
    VVipers::_implLogInfo(true, std::source_location::current(), __VA_ARGS__)
#define tagWarning(...) \
    VVipers::_implLogWarning(true, std::source_location::current(), __VA_ARGS__)
#define tagError(...) \
    VVipers::_implLogError(true, std::source_location::current(), __VA_ARGS__)
#else
#define logInfo(...) \
    VVipers::_implLogInfo(false, std::source_location(), __VA_ARGS__)
#define logWarning(...) \
    VVipers::_implLogWarning(false, std::source_location(), __VA_ARGS__)
#define logError(...) \
    VVipers::_implLogError(false, std::source_location(), __VA_ARGS__)
#define tagInfo(...) \
    VVipers::_implLogInfo(true, std::source_location(), __VA_ARGS__)
#define tagWarning(...) \
    VVipers::_implLogWarning(true, std::source_location(), __VA_ARGS__)
#define tagError(...) \
    VVipers::_implLogError(true, std::source_location(), __VA_ARGS__)
#endif

}  // namespace VVipers
#endif