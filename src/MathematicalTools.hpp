
#ifndef MATHEMATICALTOOLS_HPP
#define	MATHEMATICALTOOLS_HPP

#include <vector>
#include <stdint.h>

#include "exceptions/Exception.hpp"

namespace Maths
{
   template <class UIntType>
   const UIntType gcd(const UIntType &x, const UIntType &y)
   {
      if (y == 0)
      {
         return x;
      }

      return gcd(y, x % y);
   }
   
   /* Check if two numbers are coprimes. */
   template <class UIntType>
   bool areCoprimes(const UIntType &x, const UIntType &y)
   {
      return gcd(x, y) == 1;
   }
   
   /* Calculate the inverse of a modulo b in Z_b.
      Theorem of Bézout : a*u + b*v = 1. v is the inverse to find. */
   template <class IntType>
   const IntType getModInverse(IntType a, const IntType &b)
   {
      if(gcd(a, b) != 1)
      {
         throw Exception("The inverse of 'a' doesn't exist in Z_b. To exist, 'a' and 'b' must be coprimes.");
      }
      // If a and b are coprimes, then a^-1 exists in Z_b.
      IntType v = 0;
      IntType i = b, u = 1;
      while (a > 0)
      {
         IntType q = i / a, x = a;
         a = i % x;
         i = x;
         x = u;
         u = v - q * x;
         v = x;
      }
      v %= b;
      v = (v + b) % b;

      return v;
   }
}

// Fonctions mathématiques.
uint32_t GCD(const uint32_t a, const uint32_t b);

int32_t getModInverse(int32_t a, const int32_t n);

uint64_t getLegendreSymbol(const uint64_t &x, const uint64_t &e, const uint64_t &n);

bool isPerfectSquare(const uint32_t value);

uint8_t getByteSqrt(const uint8_t sqr);

uint16_t multiplyShort(const uint16_t a, const uint16_t b);

uint16_t inverseMultiplyShort(const uint16_t value);

#endif