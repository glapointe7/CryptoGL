#include "Adler32.hpp"

using namespace CryptoGL;

uint32_t Adler32::encode(const BytesVector &input)
{
    uint16_t a = 1, b = 0;
    for (const auto byte : input)
    {
        a = (a + byte) % mod_prime;
        b = (b + a) % mod_prime;
    }

    return (b << 16) | a;
}