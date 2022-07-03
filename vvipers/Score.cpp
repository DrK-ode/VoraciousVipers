#include <vvipers/Score.hpp>

namespace VVipers {

std::ostream& operator<<(std::ostream& os, const Score& s) {
    return os << "Score: " << s.getScore();
};

}