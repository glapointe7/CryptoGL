#include "MathematicalTools.hpp"

#include <cmath>
#include <map>

#include "exceptions/Exception.hpp"

/* Return the Greatest Common Divisor of a and b. */
uint32_t GCD(const uint32_t a, const uint32_t b)
{
   if (b == 0)
      return a;

   return GCD(b, a % b);
}

/* Calculate the inverse of a modulo b in Z_b.
Theorem of Bézout : a*u + b*v = 1. v is the inverse to find. */

int32_t getModInverse(int32_t a, const int32_t b)
{
   // If a and b are coprimes, then a^-1 exists in Z_b.
   int32_t v = 0;
   if (GCD(a, b) == 1)
   {
      int32_t i = b, u = 1;
      while (a > 0)
      {
         int32_t q = i / a, x = a;
         a = i % x;
         i = x;
         x = u;
         u = v - q * x;
         v = x;
      }
      v %= b;
      v = (v + b) % b;
   }
   else
   {
      throw Exception("The inverse of 'a' doesn't exist in Z_b. To exist, 'a' and 'b' must be coprimes.");
   }

   return v;
}

uint64_t getLegendreSymbol(const uint64_t &x, const uint64_t &e, const uint64_t &n)
{
   uint64_t a = 1;
   uint8_t e_size = sizeof (e);
   for (char i = e_size; i >= 0; --i)
   {
      a = (a * a) % n;
      if ((e & (1ull << i)) > 0)
      {
         a = (a * x) % n;
      }
   }

   return a;
}

// All perfect squares in base 16 end by the following digit : 0,1,4 or 9.

bool isPerfectSquare(const uint32_t value)
{
   const uint32_t h = value & 0xF;
   if (h > 9)
   {
      return false;
   }

   if (h != 2 && h != 3 && h != 5 && h != 6 && h != 7 && h != 8)
   {
      const uint32_t t = static_cast<uint32_t> (floor(sqrt(static_cast<double> (value)) + 0.5));
      return value == t * t;
   }

   return false;
}

uint8_t getByteSqrt(const uint8_t sqr)
{
   const std::map<uint8_t, uint8_t> byte_squares({
      {1, 1},
      {4, 2},
      {9, 3},
      {16, 4},
      {25, 5},
      {36, 6},
      {49, 7},
      {64, 8},
      {81, 9},
      {100, 10},
      {121, 11},
      {144, 12},
      {169, 13},
      {196, 14},
      {225, 15}
   });

   return byte_squares.find(sqr)->second;
}

/* Multiply a by b modulo 2^16 + 1. */
uint16_t multiplyShort(const uint16_t a, const uint16_t b)
{
   const uint32_t ab = a * b;
   if (ab != 0)
   {
      const uint16_t lo = ab & 0xFFFF;
      const uint16_t hi = (ab >> 16) & 0xFFFF;
      return (lo - hi) + (lo < hi ? 1 : 0);
   }
   
   if (a != 0)
   {
      return 1 - a;
   }
      
   return 1 - b;
}

uint16_t inverseMultiplyShort(const uint16_t value)
{
   if (value <= 1)
   {
      return value; 
   }
   
   uint16_t t1 = 0x10001 / value; 
   uint16_t y = 0x10001 % value;
   if (y == 1)
   {
      return (1 - t1) & 0xFFFF;
   }
   
   uint16_t x = value;
   uint16_t t0 = 1;
   do
   {
      uint16_t q = x / y;
      x %= y;
      t0 += q * t1;
      if (x == 1)
      {
         return t0;
      }
      q = y / x;
      y %= x;
      t1 += q * t0;
   } while (y != 1);
   
   return (1 - t1) & 0xFFFF;
}