#ifndef VVIPERS_SCORE_HPP
#define VVIPERS_SCORE_HPP

#include <stdint.h>

#include <iostream>

class Score {
  public:
    Score(uint32_t s = 0) : m_score(s){};
    uint32_t getScore() const { return m_score; }
    Score& operator=(const Score& s) {
        this->m_score += s.m_score;
        return *this;
    }
    Score operator+(const Score& s) const { return this->m_score + s.m_score; }
    Score& operator+=(const Score& s) {
        this->m_score += s.m_score;
        return *this;
    }
    Score operator-(const Score& s) const { return this->m_score - s.m_score; }
    Score& operator-=(const Score& s) {
        this->m_score -= s.m_score;
        return *this;
    }
    Score operator*(const Score& s) const { return this->m_score * s.m_score; }
    Score& operator*=(const Score& s) {
        this->m_score *= s.m_score;
        return *this;
    }
    Score operator/(const Score& s) const { return this->m_score / s.m_score; }
    Score& operator/=(const Score& s) {
        this->m_score /= s.m_score;
        return *this;
    }
    operator uint32_t() const { return m_score; }

  private:
    uint32_t m_score;
};

#endif