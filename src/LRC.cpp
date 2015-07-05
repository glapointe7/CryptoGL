#include "LRC.hpp"

using namespace CryptoGL;

uint8_t LRC::encode(const BytesVector &input)
{
    uint8_t sum = 0;
    for (const auto byte : input)
    {
        sum ^= byte;
    }

    return sum;
}