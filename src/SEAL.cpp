#include "SEAL.hpp"

#include "Endian.hpp"

#include "exceptions/BadKeyLength.hpp"

#include <limits>

using namespace CryptoGL;

std::array<uint32_t, 256> SEAL::S = {};
std::array<uint32_t, 512> SEAL::T = {};
UInt32Vector SEAL::R;

void SEAL::setKey(const BytesVector &key)
{
    if (key.size() != 20)
    {
        throw BadKeyLength("Your key has to have 20 bytes length.", key.size());
    }

    this->key = key;
}

uint32_t SEAL::gamma(SHA1 &G, UInt32Vector &block, const uint32_t index, uint32_t &previous_index)
{
    const uint32_t current_index = index / 5;
    if (current_index != previous_index)
    {
        // Get the block = (i/5) concatenated with 60 zeros.
        block.front() = current_index;

        // Apply G_a(i) to the block.
        H = IV;
        G.setCurrentBlock(block);
        G.compress(H);
        block = G.getCurrentBlock();
        previous_index = current_index;
    }

    return H[index % 5];
}

void SEAL::keySetup()
{
    // The key is the new IV for SHA-1.
    IV = BigEndian32::toIntegersVector(key);
    uint32_t previous_index = std::numeric_limits<uint32_t>::max();
    SHA1 G;

    /* Block of 16 bytes will be used by the gamma function. */
    UInt32Vector block;
    block.reserve(16);

    // Initialize tables R, S and T.
    for (uint16_t i = 0; i < 512; ++i)
    {
        T[i] = gamma(G, block, i, previous_index);
    }

    for (uint16_t i = 0; i < 256; ++i)
    {
        S[i] = gamma(G, block, i + 4096, previous_index);
    }

    const uint16_t upper_bound = 4 * (output_size / 1024);
    R.resize(upper_bound);
    for (uint16_t i = 0; i < upper_bound; ++i)
    {
        R[i] = gamma(G, block, i + 8192, previous_index);
    }
}

void SEAL::applyFourRounds()
{
    for (uint8_t j = 0; j < 4; ++j)
    {
        const uint16_t P = state[j] & 0x7FC;
        state[(j + 1) % 4] += T[P / 4];
        state[j] = uint32::rotateRight(state[j], 9);
    }
}

void SEAL::initialize()
{
    for (uint8_t i = 0; i < 4; ++i)
    {
        state[i] = uint32::rotateRight(seed, i * 8) ^ R[4 * counter + i];
    }

    for (uint8_t i = 0; i < 2; ++i)
    {
        applyFourRounds();
    }

    state[4] = state[3];
    state[5] = state[1];
    state[6] = state[0];
    state[7] = state[2];

    applyFourRounds();
}

UInt32Vector SEAL::generateKeystream()
{
    initialize();

    UInt32Vector keystream;
    keystream.reserve(1024);

    counter++;
    constexpr uint16_t MOD_CONSTANT = 0x7FC;
    for (uint8_t i = 0; i < 64; ++i)
    {
        // P and Q are round numbers as per spec.
        // 8 rounds are performed. 
        // Since the operators switch between ^ and + and they are dependencies between each state
        // it's quite complicated to create a method 'applyRound' to avoid repeating code.
        uint16_t P = state[0] & MOD_CONSTANT;
        state[1] += T[P / 4];
        state[0] = uint32::rotateRight(state[0], 9);
        state[1] ^= state[0];

        uint16_t Q = state[1] & MOD_CONSTANT;
        state[2] ^= T[Q / 4];
        state[1] = uint32::rotateRight(state[1], 9);
        state[2] += state[1];

        P = (P + state[2]) & MOD_CONSTANT;
        state[3] += T[P / 4];
        state[2] = uint32::rotateRight(state[2], 9);
        state[3] ^= state[2];

        Q = (Q + state[3]) & MOD_CONSTANT;
        state[0] ^= T[Q / 4];
        state[3] = uint32::rotateRight(state[3], 9);
        state[0] += state[3];


        P = (P + state[0]) & MOD_CONSTANT;
        state[1] ^= T[P / 4];
        state[0] = uint32::rotateRight(state[0], 9);

        Q = (Q + state[1]) & MOD_CONSTANT;
        state[2] += T[Q / 4];
        state[1] = uint32::rotateRight(state[1], 9);

        P = (P + state[2]) & MOD_CONSTANT;
        state[3] ^= T[P / 4];
        state[2] = uint32::rotateRight(state[2], 9);

        Q = (Q + state[3]) & MOD_CONSTANT;
        state[0] += T[Q / 4];
        state[3] = uint32::rotateRight(state[3], 9);

        const uint8_t j = 4 * i;
        keystream.push_back(state[1] + S[j]);
        keystream.push_back(state[2] ^ S[j + 1]);
        keystream.push_back(state[3] + S[j + 2]);
        keystream.push_back(state[0] ^ S[j + 3]);

        if (i % 2)
        {
            state[0] += state[6];
            state[1] += state[7];
            state[2] ^= state[6];
            state[3] ^= state[7];
        }
        else
        {
            state[0] += state[4];
            state[1] += state[5];
            state[2] ^= state[4];
            state[3] ^= state[5];
        }
    }

    return keystream;
}

UInt32Vector SEAL::generate()
{
    UInt32Vector random_numbers;
    random_numbers.reserve(output_size);
    const uint8_t number_of_Kb = output_size / 1024;

    while (counter < number_of_Kb)
    {
        random_numbers.extend(generateKeystream());
    }

    return random_numbers;
}