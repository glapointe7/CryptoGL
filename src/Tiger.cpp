#include "Tiger.hpp"

#include <algorithm>

using namespace CryptoGL;

constexpr std::array<std::array<uint64_t, 256>, 4> Tiger::sbox;

void Tiger::applyKeySchedule(UInt64Vector &words)
{
    constexpr uint64_t MAGIC_CONSTANT = 0xA5A5A5A5A5A5A5A5;
    words[0] -= words[7] ^ MAGIC_CONSTANT;
    words[1] ^= words[0];
    words[2] += words[1];
    words[3] -= words[2] ^ ((~words[1]) << 19);
    words[4] ^= words[3];
    words[5] += words[4];
    words[6] -= words[5] ^ ((~words[4]) >> 23);
    words[7] ^= words[6];
    words[0] += words[7];
    words[1] -= words[0] ^ ((~words[7]) << 19);
    words[2] ^= words[1];
    words[3] += words[2];
    words[4] -= words[3] ^ ((~words[2]) >> 23);
    words[5] ^= words[4];
    words[6] += words[5];
    words[7] -= words[6] ^ REGISTER_A_CONSTANT;
}

void Tiger::applyRound(const uint64_t &word, const uint8_t mult)
{
    registers[2] ^= word;
    registers[0] -= sbox[0][registers[2] & 0xFF] ^ sbox[1][(registers[2] >> 16) & 0xFF] ^
            sbox[2][(registers[2] >> 32) & 0xFF] ^ sbox[3][(registers[2] >> 48) & 0xFF];

    registers[1] += sbox[3][(registers[2] >> 8) & 0xFF] ^ sbox[2][(registers[2] >> 24) & 0xFF] ^
            sbox[1][(registers[2] >> 40) & 0xFF] ^ sbox[0][(registers[2] >> 56) & 0xFF];

    registers[1] *= mult;
}

void Tiger::pass(const UInt64Vector &words, const uint8_t mult)
{
    for (uint8_t i = 0; i < 8; ++i)
    {
        applyRound(words[i], mult);
        std::rotate(registers.begin(), registers.begin() + 1, registers.end());
    }
    std::rotate(registers.begin(), registers.begin() + 1, registers.end());
}

void Tiger::feedForward(const UInt64Vector &saved_registers)
{
    registers[0] ^= saved_registers[0];
    registers[1] -= saved_registers[1];
    registers[2] += saved_registers[2];
}

void Tiger::compress(UInt64Vector &int_block, UInt64Vector &state)
{
    registers = state;

    // Since we have 3 elements in the vector, the "+2" means that we rotate right of 1 element.
    pass(int_block, 5);
    applyKeySchedule(int_block);
    std::rotate(registers.begin(), registers.begin() + 2, registers.end());

    pass(int_block, 7);
    applyKeySchedule(int_block);
    std::rotate(registers.begin(), registers.begin() + 2, registers.end());

    pass(int_block, 9);
    std::rotate(registers.begin(), registers.begin() + 2, registers.end());

    feedForward(state);

    state = registers;
}

BytesVector Tiger::getOutput(const UInt64Vector &hash) const
{
    BytesVector output;
    output.reserve(output_size);

    const uint8_t out_data_size = output_size / 8;
    for (uint8_t j = 0; j < out_data_size; ++j)
    {
        output.extend(LittleEndian64::toBytesVector(hash[j]));
    }

    if (output_size % 8)
    {
        const BytesVector bytes = LittleEndian64::toBytesVector(hash[out_data_size]);
        output.insert(output.end(), bytes.begin(), bytes.begin() + 4);
    }

    return output;
}