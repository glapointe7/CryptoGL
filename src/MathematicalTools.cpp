#include "MathematicalTools.hpp"

#include <cmath>
#include "exceptions/Exception.hpp"

// Retourne PGCD(a,b).

uint32_t GCD(uint32_t a, uint32_t b)
{
   if (b == 0)
      return a;

   return GCD(b, a % b);
}

// Retourne le reste d'une division enti�re.

int32_t getIntegerMod(const int32_t n, const int32_t mod)
{
   int32_t ans = n;
   int32_t x = floor((float) n / mod);
   ans -= (x * mod);
   if (ans < 0)
      ans += mod;

   return ans;
}

// Calcule l'inverse de a modulo n dans Z_b.
// Théorème de Bézout : a*u + b*v = 1. v est l'inverse recherché.

int32_t getModInverse(int32_t a, const int32_t b)
{
   // Si a et n sont copremiers, alors a^-1 existe dans Z_b.
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

   for (auto number : sequence)
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
      const uint32_t t = static_cast<uint32_t>(floor(sqrt(static_cast<double>(value)) + 0.5));
      return value == t * t;
   }
   return 0;
}