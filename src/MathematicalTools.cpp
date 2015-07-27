#include "MathematicalTools.hpp"

#include <cmath>
#include <map>

using namespace CryptoGL;

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

/* All perfect squares in base 16 end by the following digit : 0,1,4 or 9.*/
bool Maths::isPerfectSquare(const uint32_t value)
{
    const uint8_t last_digit = value % 16;
    if (last_digit > 9)
    {
        return false;
    }

    const uint8_t second_last_digit = (value >> 1) % 16;
    switch (last_digit)
    {
        case 1:
        case 9:
        {
            const uint32_t t = static_cast<uint32_t> (floor(sqrt(static_cast<double> (value)) + 0.5));
            return value == t * t;
        }
            
        case 0:
            switch (second_last_digit)
            {
                case 0:
                case 1:
                case 4:
                case 9:
                {
                    const uint32_t t = static_cast<uint32_t> (floor(sqrt(static_cast<double> (value)) + 0.5));
                    return value == t * t;
                }

                default:
                    return false;
            }

        case 4:
        {
            if (!(second_last_digit % 2))
            {
                const uint32_t t = static_cast<uint32_t> (floor(sqrt(static_cast<double> (value)) + 0.5));
                return value == t * t;
            }
            return false;
        }
    }

    return false;
}

uint8_t Maths::GFMultiply(const uint8_t a, uint8_t b, const uint16_t p)
{
    uint16_t A = a;
    uint8_t prod = 0;
    while (b > 0)
    {
        if (b & 1)
        {
            prod ^= A;
        }
        A <<= 1;
        b >>= 1;
        if (A & 0x100)
        {
            A ^= p;
        }
    }

    return prod;
}

uint8_t Maths::getByteSqrt(const uint8_t sqr)
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
uint16_t Maths::multiplyShort(const uint16_t a, const uint16_t b)
{
    const auto ab = a * b;
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

uint16_t Maths::inverseMultiplyShort(const uint16_t value)
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
    }
    while (y != 1);

    return (1 - t1) & 0xFFFF;
}