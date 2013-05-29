#include "MathematicalTools.hpp"

#include <cmath>
#include <map>
#include "exceptions/Exception.hpp"

// Return GCD(a,b).

uint32_t GCD(uint32_t a, uint32_t b)
{
   if (b == 0)
      return a;

   return GCD(b, a % b);
}

// Calcule l'inverse de a modulo n dans Z_b.
// Theorem of Bézout : a*u + b*v = 1. v is the inverse to find.

int32_t getModInverse(int32_t a, const int32_t b)
{
   // If a and n are coprimes, then a^-1 exists in Z_b.
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

bool isSuperIncresing(const std::vector<uint64_t> &sequence)
{
   uint64_t sum = 0;

   for (const auto number : sequence)
   {
      if (number > sum)
      {
         sum += number;
      }
      else
      {
         return false;
      }
   }

   return true;
}

uint64_t getLegendreSymbol(const uint64_t x, const uint64_t e, const uint64_t n)
{
   uint64_t a = 1;
   unsigned char e_size = sizeof (e);
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

// Tout carré parfait en base 16 se termine par 0,1,4 ou 9.

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
   return 0;
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