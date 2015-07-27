#include "Tiger.hpp"

#include <algorithm>

using namespace CryptoGL;

constexpr std::array<std::array<uint64_t, 256>, 4> Tiger::SBOX;

void Tiger::applyKeySchedule()
{
    constexpr uint64_t MAGIC_CONSTANT = 0xA5A5A5A5A5A5A5A5;
    current_block[0] -= current_block[7] ^ MAGIC_CONSTANT;
    current_block[1] ^= current_block[0];
    current_block[2] += current_block[1];
    current_block[3] -= current_block[2] ^ ((~current_block[1]) << 19);
    current_block[4] ^= current_block[3];
    current_block[5] += current_block[4];
    current_block[6] -= current_block[5] ^ ((~current_block[4]) >> 23);
    current_block[7] ^= current_block[6];
    current_block[0] += current_block[7];
    current_block[1] -= current_block[0] ^ ((~current_block[7]) << 19);
    current_block[2] ^= current_block[1];
    current_block[3] += current_block[2];
    current_block[4] -= current_block[3] ^ ((~current_block[2]) >> 23);
    current_block[5] ^= current_block[4];
    current_block[6] += current_block[5];
    current_block[7] -= current_block[6] ^ REGISTER_A_CONSTANT;
}

void Tiger::applyRound(const uint64_t &word, const uint8_t mult)
{
    registers[2] ^= word;
    registers[0] -= SBOX[0][registers[2] & 0xFF] ^ SBOX[1][(registers[2] >> 16) & 0xFF] ^
            SBOX[2][(registers[2] >> 32) & 0xFF] ^ SBOX[3][(registers[2] >> 48) & 0xFF];

    registers[1] += SBOX[3][(registers[2] >> 8) & 0xFF] ^ SBOX[2][(registers[2] >> 24) & 0xFF] ^
            SBOX[1][(registers[2] >> 40) & 0xFF] ^ SBOX[0][(registers[2] >> 56) & 0xFF];

    registers[1] *= mult;
}

void Tiger::pass(const uint8_t mult)
{
    for (uint8_t i = 0; i < 8; ++i)
    {
        applyRound(current_block[i], mult);
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

void Tiger::compress(UInt64Vector &state)
{
    registers = state;

    // Since we have 3 elements in the vector, the "+2" means that we rotate right of 1 element.
    pass(5);
    applyKeySchedule();
    std::rotate(registers.begin(), registers.begin() + 2, registers.end());

    pass(7);
    applyKeySchedule();
    std::rotate(registers.begin(), registers.begin() + 2, registers.end());

    pass(9);
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
        output.extend(bytes, 0, 4);
    }

    return output;
}