#include "MathematicalTools.hpp"

// Retourne PGCD(a,b).

uint_fast32_t getPGCD(uint_fast32_t a, uint_fast32_t b)
{
   if (b == 0)
      return a;

   return getPGCD(b, a % b);
}

// Retourne le reste d'une division enti�re.

long getIntegerMod(const long n, const long mod)
{
   long ans = n;
   long x = floor((float) n / mod);
   ans -= (x * mod);
   if (ans < 0)
      ans += mod;

   return ans;
}

// Calcule l'inverse de a modulo n et en retourne la valeur.
// TODO : Vérifier qu'il existe a^{-1} dans Z/nZ.

long getModInverse(long a, const long n)
{
   long i = n, v = 0, d = 1;
   while (a > 0)
   {
      long t = i / a, x = a;
      a = i % x;
      i = x;
      x = d;
      d = v - t * x;
      v = x;
   }
   v %= n;
   if (v < 0) v = (v + n) % n;

   return v;
}

bool isSuperIncresing(const std::vector<unsigned long> &sequence)
{
   unsigned long sum = 0;

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
      if (e & (1ull << i) > 0)
      {
         a = (a * x) % n;
      }
   }

   return a;
}