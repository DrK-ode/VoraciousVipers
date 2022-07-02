#include "Score.hpp"

std::ostream& operator<<(std::ostream& os, const Score& s) {
    return os << "Score: " << s.getScore();
};