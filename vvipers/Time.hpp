#ifndef VVIPERS_TIME_HPP
#define VVIPERS_TIME_HPP

#include <chrono>

namespace VVipers {

typedef std::chrono::duration<double,std::nano> Time;

inline const Time seconds(double t) {return std::chrono::duration_cast<Time>(std::chrono::duration<double>(t));}
inline double toSeconds(const Time& t) {return std::chrono::duration_cast<std::chrono::duration<double>>(t).count();}

}  // namespace VVipers

#endif