
#ifndef MATHEMATICALTOOLS_HPP
#define MATHEMATICALTOOLS_HPP

#include <vector>
#include <algorithm>

#include "exceptions/Exception.hpp"

namespace Maths
{   
   /* Return the Greatest Common Divisor of x and y. */
   template <typename UIntType>
   UIntType gcd(const UIntType &x, const UIntType &y)
   {
      if (y == 0)
      {
         return x;
      }

      return gcd(y, x % y);
   }

   /* Check if x and y are coprimes. */
   template <typename UIntType>
   bool areCoprimes(const UIntType &x, const UIntType &y)
   {
      return gcd(x, y) == 1;
   }
   
   /* Calculate the Euler's Totient function. */
   template <typename UInteger>
   UInteger phi(const UInteger &x)
   {
      UInteger sum = 1;
      for(UInteger i = 2; i < x; ++i)
      {
         if(areCoprimes(i, x))
         {
            sum++;
         }
      }
      
      return sum;
   }

   /* Calculate the inverse of a modulo b in Z_b.
      Theorem of Bézout : a*u + b*v = 1. v is the inverse to find. */
   template <typename IntType>
   IntType getModInverse(IntType a, const IntType &b)
   {
      if (!areCoprimes(a, b))
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

   /* Calculate the square root of a big integer. */
   template <typename UIntType>
   UIntType integerSqrt(const UIntType &n)
   {
      UIntType x = n;
      UIntType y = n;
      y += 1;
      y >>= 1;
      while (y < x)
      {
         x = y;
         y += n / x;
         y >>= 1;
      }

      return x;
   }

   /* Test the primality of x. */
   template <typename UIntType>
   bool isPrime(const UIntType &x)
   {
      if (x < 2) return false;
      if (x < 4) return true;
      if (x % 2 == 0) return false;

      const UIntType upper_bound = Maths::integerSqrt(x) + 1;
      for (UIntType i = 3; i <= upper_bound; i += 2)
      {
         if (x % i == 0) return false;
      }

      return true;
   }
   
   /* Get the modulo for positive or negative numbers. */
   template <typename Integer>
   Integer mod(const Integer value, const uint64_t &modulo)
   {
      return (value + modulo) % modulo;
   }

   /* Check if 'value' is a perfect square. */
   bool isPerfectSquare(const uint32_t value);
   
   /* Multiply a by b in the Galois Field GF(2^8) with the polynom p. */
   uint8_t GFMultiply(const uint8_t a, uint8_t b, const uint16_t p);
}

//uint64_t getLegendreSymbol(const uint64_t &x, const uint64_t &e, const uint64_t &n);

uint8_t getByteSqrt(const uint8_t sqr);

uint16_t multiplyShort(const uint16_t a, const uint16_t b);

uint16_t inverseMultiplyShort(const uint16_t value);

#endif