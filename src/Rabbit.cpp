#include "Rabbit.hpp"

#include "Bits.hpp"
#include "exceptions/BadKeyLength.hpp"

using namespace CryptoGL;

constexpr std::array<uint32_t, 8> Rabbit::A;

void Rabbit::setIV(const BytesVector &IV)
{
    const uint8_t iv_size = IV.size();
    if (iv_size != 8 && iv_size != 0)
    {
        throw BadIVLength("Your IV has to be 8 bytes length.", iv_size);
    }

    this->IV = IV;

    if (!IV.empty())
    {
        IVSetup();
    }
}

constexpr uint32_t Rabbit::g(const uint32_t x)
{
    // Construct high and low argument for squaring.
    const uint32_t a = x & 0xFFFF;
    const uint32_t b = x >> 16;

    // Calculate high and low result of squaring.
    const uint32_t h = ((((a * a) >> 17) + (a * b)) >> 15) + (b * b);

    // Return high XOR low. 
    return h ^ (x * x);
}

void Rabbit::nextState()
{
    const std::array<uint32_t, 8> old_counters(counters);

    // Calculate new counter values.
    counters[0] += A[0] + counter_carry_bit;
    for (uint8_t i = 1; i < 8; ++i)
    {
        counters[i] += A[i] + (counters[i - 1] < old_counters[i - 1]);
    }
    counter_carry_bit = (counters[7] < old_counters[7]);

    std::array<uint32_t, 8> G;
    for (uint8_t i = 0; i < 8; ++i)
    {
        G[i] = g(states[i] + counters[i]);
    }

    // Calculate new state values.
    for (uint8_t i = 0; i < 8; i += 2)
    {
        states[i] = G[i] + Bits::rotateLeft(G[(i + 7) % 8], 16) + Bits::rotateLeft(G[(i + 6) % 8], 16);
        states[i + 1] = G[i + 1] + Bits::rotateLeft(G[i], 8) + G[(i + 7) % 8];
    }
}

void Rabbit::keySetup()
{
    // Build 4 sub-keys of 4 bytes length.
    const UInt32Vector subkeys = BigEndian32::toIntegersVector(key);

    /* Generate initial state variables */
    constexpr uint8_t SHIFT_CONSTANT = 16;
    constexpr uint32_t HIGH_UINT_MOD_CONSTANT = 0xFFFF0000;
    constexpr uint32_t LOW_UINT_MOD_CONSTANT = 0xFFFF;
    states[0] = subkeys[0];
    states[2] = subkeys[1];
    states[4] = subkeys[2];
    states[6] = subkeys[3];
    states[1] = (subkeys[3] << SHIFT_CONSTANT) | (subkeys[2] >> SHIFT_CONSTANT);
    states[3] = (subkeys[0] << SHIFT_CONSTANT) | (subkeys[3] >> SHIFT_CONSTANT);
    states[5] = (subkeys[1] << SHIFT_CONSTANT) | (subkeys[0] >> SHIFT_CONSTANT);
    states[7] = (subkeys[2] << SHIFT_CONSTANT) | (subkeys[1] >> SHIFT_CONSTANT);

    /* Generate initial counter values */
    counters[0] = Bits::rotateLeft(subkeys[2], SHIFT_CONSTANT);
    counters[2] = Bits::rotateLeft(subkeys[3], SHIFT_CONSTANT);
    counters[4] = Bits::rotateLeft(subkeys[0], SHIFT_CONSTANT);
    counters[6] = Bits::rotateLeft(subkeys[1], SHIFT_CONSTANT);
    counters[1] = (subkeys[0] & HIGH_UINT_MOD_CONSTANT) | (subkeys[1] & LOW_UINT_MOD_CONSTANT);
    counters[3] = (subkeys[1] & HIGH_UINT_MOD_CONSTANT) | (subkeys[2] & LOW_UINT_MOD_CONSTANT);
    counters[5] = (subkeys[2] & HIGH_UINT_MOD_CONSTANT) | (subkeys[3] & LOW_UINT_MOD_CONSTANT);
    counters[7] = (subkeys[3] & HIGH_UINT_MOD_CONSTANT) | (subkeys[0] & LOW_UINT_MOD_CONSTANT);

    counter_carry_bit = 0;

    for (uint8_t i = 0; i < 4; ++i)
    {
        nextState();
    }

    for (uint8_t i = 0; i < 8; ++i)
    {
        counters[i] ^= states[(i + 4) % 8];
    }
}

void Rabbit::IVSetup()
{
    // Generate 4 sub-IVs of 16 bits big-endian.
    const UInt16Vector subIV = BigEndian16::toIntegersVector(IV);

    /* Modify counter values */
    for (uint8_t i = 0; i < 8; ++i)
    {
        counters[i] ^= subIV[i % 4];
    }

    for (uint8_t i = 0; i < 4; ++i)
    {
        nextState();
    }
}

UInt32Vector Rabbit::generateKeystream()
{
    nextState();
    constexpr uint8_t SHIFT_CONSTANT = 16;

    return {states[0] ^ (states[5] >> SHIFT_CONSTANT) ^ (states[3] << SHIFT_CONSTANT),
            states[2] ^ (states[7] >> SHIFT_CONSTANT) ^ (states[5] << SHIFT_CONSTANT),
            states[4] ^ (states[1] >> SHIFT_CONSTANT) ^ (states[7] << SHIFT_CONSTANT),
            states[6] ^ (states[3] >> SHIFT_CONSTANT) ^ (states[1] << SHIFT_CONSTANT)
    };
}