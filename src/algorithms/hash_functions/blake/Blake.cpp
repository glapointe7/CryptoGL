#include "Blake.hpp"

using namespace CryptoGL;

template <>
const std::array<uint64_t, 16> ArrayGetter<uint64_t>::G = {{
    0x243F6A8885A308D3, 0x13198A2E03707344, 0xA4093822299F31D0, 0x082EFA98EC4E6C89,
    0x452821E638D01377, 0xBE5466CF34E90C6C, 0xC0AC29B7C97C50DD, 0x3F84D5B5B5470917,
    0x9216D5D98979FB1B, 0xD1310BA698DFB5AC, 0x2FFD72DBD01ADFB7, 0xB8E1AFED6A267E96,
    0xBA7C9045F12C7F99, 0x24A19947B3916CF7, 0x0801F2E2858EFC16, 0x636920D871574E69
}};

template <>
const std::array<uint32_t, 16> ArrayGetter<uint32_t>::G = {{
    0x243F6A88, 0x85A308D3, 0x13198A2E, 0x03707344,
    0xA4093822, 0x299F31D0, 0x082EFA98, 0xEC4E6C89,
    0x452821E6, 0x38D01377, 0xBE5466CF, 0x34E90C6C,
    0xC0AC29B7, 0xC97C50DD, 0x3F84D5B5, 0xB5470917
}};

template <typename DataType, uint8_t InputBlockSize>
constexpr std::array<std::array<uint8_t, 16>, 10> Blake<DataType, InputBlockSize>::sigma;

BytesVector Blake256::pad(BytesVector message) const
{
    const uint64_t initial_length = message.size();
    //BytesVector padding(message);
    message.reserve(initial_length + 128);

    // If initial_length + 1 is congruent to 56 (mod 64).
    if (((initial_length + 1) % 64) == 56)
    {
        message.push_back(0x81);
    }
    else
    {
        message.push_back(0x80);

        const uint8_t zeros = (120 - (message.size() & 0x3F)) & 0x3F;
        message.insert(message.end(), zeros, 0);
        message.back() = 0x01;
    }

    return message;
}

BytesVector Blake512::pad(BytesVector message) const
{
    const uint64_t initial_length = message.size();
    //BytesVector padding(message);
    message.reserve(initial_length + 256);

    // If initial_length + 1 is congruent to 112 (mod 128).
    if (((initial_length + 1) % 128) == 112)
    {
        message.push_back(0x81);
    }
    else
    {
        message.push_back(0x80);

        const uint16_t zeros = (240 - (message.size() & 0x7F)) & 0x7F;
        message.insert(message.end(), zeros, 0);
        message.back() = 0x01;
    }

    // Append 8 times 0x00 byte because we don't support 128-bit integer.
    message.insert(message.end(), 8, 0);

    return message;
}