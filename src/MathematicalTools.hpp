
#ifndef MATHEMATICALTOOLS_HPP
#define	MATHEMATICALTOOLS_HPP

#include <vector>
#include <stdint.h>

// Fonctions mathématiques.
uint32_t getPGCD(uint32_t a, uint32_t b);
long getIntegerMod(const long n, const long mod);
long getModInverse(long a, const long n);
bool isSuperIncresing(const std::vector<unsigned long> &sequence);
uint64_t getLegendreSymbol(const uint64_t x, const uint64_t e, const uint64_t n);

#endif

