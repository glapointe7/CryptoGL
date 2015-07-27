#include "Adler32.hpp"

using namespace CryptoGL;

uint32_t Adler32::encode(const BytesVector &input)
{
    uint16_t a = 1, b = 0;
    for (const auto byte : input)
    {
        a = (a + byte) % MOD_PRIME;
        b = (b + a) % MOD_PRIME;
    }

    return (b << 16) | a;
}