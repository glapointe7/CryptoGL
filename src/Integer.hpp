
#ifndef INTEGER_HPP
#define INTEGER_HPP

#include "String.hpp"
#include <algorithm>

namespace CryptoGL
{
    template <typename UInteger>
    class Integer
    {
        static_assert(std::is_integral<UInteger>::value, "Type UInteger must be an integral type.");

    public:
        /* Extract the Least Signifiant Bits from value. */
        static constexpr UInteger lsb(const UInteger value, const uint8_t bits_to_extract)
        {
            return value & (static_cast<UInteger> (1) << bits_to_extract);
        }
        
        /* Extract the Most Signifiant Bits from value. */
        static constexpr UInteger msb(const UInteger value, const uint8_t bits_to_extract)
        {
            return value >> ((sizeof (UInteger) * 8) - bits_to_extract);
        }
        
        /* Rotate left 'value' of 'shift' bits with 'max' in {1,...,63}. */
        static constexpr UInteger rotateLeft(const UInteger value, const uint8_t shift, const uint8_t max = 32)
        {
            return ((value << shift) | (value >> (max - shift)));
        }
        
        /* Rotate left 'value' of 'shift' bits with 'max' in {1,...,63}. */
        static constexpr UInteger rotateRight(const UInteger value, const uint8_t shift, const uint8_t max = 32)
        {
            return ((value >> shift) | (value << (max - shift)));
        }
        
        /* Test a bit at a 'pos' in 'value'. */
        static constexpr bool getBitAtPosition(const UInteger value, const uint8_t pos)
        {
            return (value & (1ull << pos)) > 0;
        }
        
        /* Set a bit at 'pos' in 'value'. */
        static constexpr UInteger setBitAtPosition(const UInteger value, const uint8_t pos)
        {
            return value | (1ull << pos);
        }
        
        /* Convert an integer 'value' to a string. */
        static String toString(UInteger value)
        {
            String str;
            str.reserve(20);
            do
            {
                str.push_back(static_cast<char> (48 + (value % 10)));
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