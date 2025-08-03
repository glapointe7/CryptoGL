#pragma once

#include "Types.hpp"

namespace CryptoGL
{
    namespace Padding
    {
        /* Pad 'data' with 'block_size' times 0x0. */
        BytesVector zeros(BytesVector data, const uint8_t block_size);

        /* Add the 'value' of the number of remaining byte 'value' times. */
        BytesVector remainingValue(BytesVector data, const uint8_t block_size);

        /* Apply Pad10* rule from the specs. 1 bit upto block_size bits are appended. */
        BytesVector _10Star(BytesVector data, const uint8_t block_size);

        /* Apply padding Pad10*1 rule from the specs. 2 bits upto block_size + 1 bits are appended. */
        BytesVector _10Star1(BytesVector data, const uint8_t block_size,
                const uint8_t first_byte = 0x01, const uint8_t last_byte = 0x80);
    }
}