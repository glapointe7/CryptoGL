#ifndef INTEGER_HPP
#define INTEGER_HPP

#include "String.hpp"
#include <algorithm>
#include <assert.h>
#include <limits>

namespace CryptoGL
{
    template <typename UInteger>
    class Integer
    {
        static_assert(std::is_integral<UInteger>::value, "Integer: The type UInteger must be an integral type.");

    public:
        /* Extract the Least Significant Bits from value. */
        static constexpr UInteger lsb(const UInteger value, const uint8_t bits_to_extract)
        {
            assert(bits_to_extract <= sizeof(UInteger) * 8 && "Integer (lsb): The argument 'bits_to_extract' has to be between 0 and 63.");
            return value & (static_cast<UInteger> (1) << bits_to_extract);
        }
        
        /* Extract the Most Significant Bits from value. */
        static constexpr UInteger msb(const UInteger value, const uint8_t bits_to_extract)
        {
            assert(bits_to_extract <= sizeof(UInteger) * 8 && "Integer (msb): The argument 'bits_to_extract' has to be between 0 and 63.");
            return value >> ((sizeof (UInteger) * 8) - bits_to_extract);
        }
        
        /* Rotate left 'value' of 'shift' bits. 
         */
        static constexpr UInteger rotateLeft(const UInteger value, const uint8_t shift)
        {
            constexpr uint8_t uint_size = sizeof(UInteger) * 8;
            assert(shift <= uint_size && "Integer (rotateLeft): The argument 'shift' has to be between less or equals to uint_size.");
            return ((value << shift) | (value >> (uint_size - shift)));
        }
        
        /* Rotate left 'value' of 'shift' bits with 'max' in {1,...,64}. */
        static constexpr UInteger rotateLeft(const UInteger value, const uint8_t shift, const uint8_t max)
        {
            constexpr uint8_t uint_size = sizeof(UInteger) * 8;
            assert((shift <= uint_size && max <= uint_size) && "Integer (rotateLeft): The argument 'shift' has to be between less or equals to uint_size.");
            return ((value << shift) | (value >> (max - shift))) & ((1ull << max) - 1);
        }
        
        /* Rotate right 'value' of 'shift' bits with 'max' in {1,...,64}. 
         * Default value will be adjusted with the type of integer (e.g. uint32 => max = 32).
         */
        static constexpr UInteger rotateRight(const UInteger value, const uint8_t shift, const uint8_t max = sizeof(UInteger) * 8)
        {
            assert(max <= sizeof(UInteger) * 8 && "Integer (rotateRight): The argument 'max' has to be between 1 and 64.");
            return ((value >> shift) | (value << (max - shift)));
        }
        
        /* Test a bit at a 'pos' in 'value'. */
        static constexpr bool getBitAtPosition(const UInteger value, const uint8_t pos)
        {
            assert(pos < 8 * sizeof(UInteger) && "Integer (getBitAtPosition): The argument 'pos' has to be between 0 and 7.");
            return (value & (1ull << pos)) > 0;
        }
        
        /* Set a bit at 'pos' in 'value'. */
        static constexpr UInteger setBitAtPosition(const UInteger value, const uint8_t pos)
        {
            assert(pos < 8 * sizeof(UInteger) && "Integer (setBitAtPosition): The argument 'pos' has to be between 0 and 7.");
            return value | (1ull << pos);
        }
        
        /* Extract a byte in value at position 'pos'. */
        static constexpr uint8_t getByteAtPosition(const UInteger value, const uint8_t pos)
        {
            assert(pos < sizeof(UInteger) && "Integer (getByteAtPosition): The argument 'pos' has to be between 0 and 7.");
            return (value >> (pos * 8)) & 0xFF;
        }
        
        /* Swap every byte in the integer 'value'. */
        static UInteger bytesSwap(const UInteger value)
        {
            constexpr uint8_t uint_size = sizeof(UInteger);
            uint32_t retval = value & 0xFF;
            for(uint8_t i = 1; i < uint_size; ++i)
            {
                retval = (retval << 8) | ((value >> (8 * i)) & 0xFF);
            }
            
            return retval;
        }

        static constexpr UInteger reverse_bits(UInteger n) {
            static_assert(std::is_unsigned<UInteger>::value, "reverse_bits requires unsigned integer types");

            constexpr int num_bits = std::numeric_limits<UInteger>::digits;
            UInteger result = 0;
            for (int i = 0; i < num_bits; ++i) {
                result <<= 1;
                result |= (n & 1);
                n >>= 1;
            }

            return result;
        }
        
        /* Convert an integer 'value' to a string. */
        static String toString(UInteger value)
        {
            String str;
            str.reserve(20);
            do
            {
                str.push_back(static_cast<char> ('0' + (value % 10)));
            }
            while (value /= 10);
            std::reverse(str.begin(), str.end());

            return str;
        }
    };

    using uint8 = Integer<uint8_t>;
    using uint16 = Integer<uint16_t>;
    using uint32 = Integer<uint32_t>;
    using uint64 = Integer<uint64_t>;
}

#endif