
#ifndef MATHEMATICALTOOLS_HPP
#define	MATHEMATICALTOOLS_HPP

#include <vector>
#include <stdint.h>

// Fonctions mathématiques.
uint32_t GCD(uint32_t a, uint32_t b);
int32_t getModInverse(int32_t a, const int32_t n);
bool isSuperIncresing(const std::vector<uint64_t> &sequence);
uint64_t getLegendreSymbol(const uint64_t x, const uint64_t e, const uint64_t n);
bool isPerfectSquare(const uint32_t value);
uint8_t getByteSqrt(const uint8_t sqr);

#endif