#include "MathematicalTools.hpp"

#include <cmath>
#include <map>

/*uint64_t getLegendreSymbol(const uint64_t &x, const uint64_t &e, const uint64_t &n)
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
}*/

// All perfect squares in base 16 end by the following digit : 0,1,4 or 9.

bool Maths::isPerfectSquare(const uint32_t value)
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