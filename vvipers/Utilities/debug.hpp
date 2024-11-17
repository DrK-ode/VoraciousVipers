#pragma once

#include <iostream>
#include <source_location>
#include <string_view>

namespace VVipers {

enum class Verbosity { Silent = 0, OnlyErrors, ErrorsAndWarnings, All };

namespace debug {
inline Verbosity verbosity = Verbosity::All;
}

#ifndef __INTELLISENSE__  // Because intellisense bugs on
                          // std::source_location::current() and this removes
                          // the error squiggles
inline void tag(const std::source_location& location, std::ostream& os) {
    std::string_view filename(location.file_name());
    os << '@' << filename.substr(filename.find_last_of("/\\") + 1) << ":"
       << location.line() << " " << location.function_name() << std::endl;
}
inline void info_tag(
    const std::source_location& location = std::source_location::current()) {
    if (debug::verbosity >= Verbosity::All)
        tag(location, std::cout);
}
inline void warning_tag(
    const std::source_location& location = std::source_location::current()) {
    if (debug::verbosity >= Verbosity::ErrorsAndWarnings)
        tag(location, std::cerr);
}
inline void error_tag(
    const std::source_location& location = std::source_location::current()) {
    if (debug::verbosity >= Verbosity::OnlyErrors)
        tag(location, std::cerr);
}

inline void debug_tag(
    const std::source_location& location = std::source_location::current()) {
    tag(location, std::cerr);
}
#else
inline void tag(const std::source_location& location = std::source_location(),
                std::ostream& os = std::cout) {}
inline void info_tag(
    const std::source_location& location = std::source_location()) {}
inline void warning_tag(
    const std::source_location& location = std::source_location()) {}
inline void error_tag(
    const std::source_location& location = std::source_location()) {}
inline void debug_tag(
    const std::source_location& location = std::source_location()) {}
#endif

template <typename... Args>
inline void _impl_log_info(bool doTag, const std::source_location loc,
                           Args&&... args) {
    if (debug::verbosity >= Verbosity::All) {
        if (doTag)
            tag(loc, std::cout);
        std::cout << "   INFO   ";
        (std::cout << ... << std::forward<Args>(args)) << '\n';
    }
}

template <typename... Args>
inline void _impl_log_warning(bool doTag, const std::source_location loc,
                              Args&&... args) {
    if (debug::verbosity >= Verbosity::ErrorsAndWarnings) {
        if (doTag)
            tag(loc, std::cerr);
        std::cerr << "   WARN   ";
        (std::cerr << ... << std::forward<Args>(args)) << '\n';
    }
}

template <typename... Args>
inline void _impl_log_error(bool doTag, const std::source_location loc,
                            Args&&... args) {
    if (debug::verbosity >= Verbosity::OnlyErrors) {
        if (doTag)
            tag(loc, std::cerr);
        std::cerr << "   ERR    ";
        (std::cerr << ... << std::forward<Args>(args)) << '\n';
    }
}

template <typename... Args>
inline void _impl_log_debug(bool doTag, const std::source_location loc,
                            Args&&... args) {
    if (doTag)
        tag(loc, std::cerr);
    std::cerr << "   DEBUG  ";
    (std::cerr << ... << std::forward<Args>(args)) << '\n';
}

#ifndef __INTELLISENSE__  // Because intellisense bugs on
                          // std::source_location::current() and this removes
                          // the error squiggles
#define log_info(...) \
    _impl_log_info(false, std::source_location::current(), __VA_ARGS__)
#define logWarning(...) \
    _impl_log_warning(false, std::source_location::current(), __VA_ARGS__)
#define log_error(...) \
    _impl_log_error(false, std::source_location::current(), __VA_ARGS__)
#define log_debug(...) \
    _impl_log_debug(false, std::source_location::current(), __VA_ARGS__)
#define tagInfo(...) \
    _impl_log_info(true, std::source_location::current(), __VA_ARGS__)
#define tag_warning(...) \
    _impl_log_warning(true, std::source_location::current(), __VA_ARGS__)
#define tag_error(...) \
    _impl_log_error(true, std::source_location::current(), __VA_ARGS__)
#define tag_debug(...) \
    _impl_log_Debug(true, std::source_location::current(), __VA_ARGS__)
#else
#define log_info(...) _impl_log_Info(false, std::source_location(), __VA_ARGS__)
#define log_warning(...) \
    _impl_log_warning(false, std::source_location(), __VA_ARGS__)
#define log_error(...) \
    _impl_log_error(false, std::source_location(), __VA_ARGS__)
#define log_debug(...) \
    _impl_log_debug(false, std::source_location(), __VA_ARGS__)
#define tag_info(...) _impl_log_info(true, std::source_location(), __VA_ARGS__)
#define tag_warning(...) \
    _impl_log_warning(true, std::source_location(), __VA_ARGS__)
#define tag_error(...) \
    _impl_log_error(true, std::source_location(), __VA_ARGS__)
#define tag_debug(...) \
    _impl_log_debug(true, std::source_location(), __VA_ARGS__)
#endif

}  // namespace VVipers