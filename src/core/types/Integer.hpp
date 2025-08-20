#pragma once

#include "String.hpp"
#include <algorithm>
#include <assert.h>
#include <limits>

#include <bit>
#include <concepts>
#include <type_traits>
    // Excludes bool (which is technically integral but not useful here)
namespace CryptoGL
{
    template <typename T>
    concept UnsignedIntegral = std::unsigned_integral<T> && (!std::same_as<T, bool>);
    template <UnsignedIntegral UInteger>
    class Integer
    {
        static constexpr uint8_t BITS_COUNT = sizeof(UInteger) * 8;
        static constexpr uint8_t BYTES_COUNT = sizeof(UInteger);
        static constexpr UInteger ALL_ONES = ~static_cast<UInteger>(0);

    public:
        /* Extract the Least Significant Bits from value. */
        [[nodiscard]] static constexpr UInteger lsb(const UInteger value, const uint8_t bits_to_extract) noexcept
        {
            const UInteger mask = (bits_to_extract == BITS_COUNT) ? 
                ALL_ONES : 
                (static_cast<UInteger>(1) << bits_to_extract) - 1;
            
            return value & mask;
        }
        
        /* Extract the Most Significant Bits from value. */
        [[nodiscard]] static constexpr UInteger msb(const UInteger value, const uint8_t bits_to_extract) noexcept
        {
            return value >> (BITS_COUNT - bits_to_extract);
        }
        
        /* Rotate left 'value' of 'shift' bits. 
         */
        [[nodiscard]] static constexpr UInteger rotateLeft(const UInteger value, const uint8_t shift) noexcept
        {
            return std::rotl(value, shift);
        }
        
        /* Rotate left 'value' of 'shift' bits with 'max' in {1,...,64}. */
        [[nodiscard]] static constexpr UInteger rotateLeft(const UInteger value, const uint8_t shift, const uint8_t max) noexcept
        {
            //return std::rotl(value, shift) & ((1ull << max) - 1);
            return ((value << shift) | (value >> (max - shift))) & ((1ull << max) - 1);
        }
        
        /* Rotate right 'value' of 'shift' bits with 'max' in {1,...,64}. 
         * Default value will be adjusted with the type of integer (e.g. uint32 => max = 32).
         */
        [[nodiscard]] static constexpr UInteger rotateRight(const UInteger value, const uint8_t shift, const uint8_t max = BITS_COUNT) noexcept
        {
            //assert(max <= BITS_COUNT && "Integer (rotateRight): The argument 'max' has to be between 1 and 64.");
            return ((value >> shift) | (value << (max - shift)));
        }
        
        /* Test a bit at a 'pos' in 'value'. */
        [[nodiscard]] static constexpr bool getBitAtPosition(const UInteger value, const uint8_t pos) noexcept
        {
            return (value & (1ull << pos)) > 0;
        }
        
        /* Set a bit at 'pos' in 'value'. */
        [[nodiscard]] static constexpr UInteger setBitAtPosition(const UInteger value, const uint8_t pos) noexcept
        {
            return value | (1ull << pos);
        }
        
        /* Extract a byte in value at position 'pos'. */
        [[nodiscard]] static constexpr uint8_t getByteAtPosition(const UInteger value, const uint8_t pos) noexcept
        {
            return (value >> (pos * 8)) & 0xFF;
        }
        
        /* Swap every byte in the integer 'value'. */
        [[nodiscard]] static constexpr UInteger bytesSwap(UInteger value) noexcept
        {
            UInteger result = 0;
            for (std::size_t i = 0; i < BYTES_COUNT; ++i) {
                result <<= 8;
                result |= (value & 0xFF);
                value >>= 8;
            }

            return result;
        }
        
        /* Convert an integer 'value' to a string. */
        [[nodiscard]] static String toString(UInteger value) noexcept
        {
            constexpr int MAX_DIGITS = std::numeric_limits<UInteger>::digits10 + 1;
            std::array<char, MAX_DIGITS> buf{};
            UInteger tmp = value;

            for (int i = MAX_DIGITS - 1; i >= 0; --i) {
                buf[i] = static_cast<char>('0' + (tmp % 10));
                tmp /= 10;
            }

            // Trouver l'index du premier chiffre non '0' de manière branchless
            int idx = 0;
            bool seen_nonzero = false;
            for (int i = 0; i < MAX_DIGITS - 1; ++i) {
                bool is_nonzero = (buf[i] != '0');
                seen_nonzero = seen_nonzero || is_nonzero;
                if (!seen_nonzero) idx++;
            }

            return std::string(buf.begin() + idx, buf.end());
        }
    };

    using uint8 = Integer<uint8_t>;
    using uint16 = Integer<uint16_t>;
    using uint32 = Integer<uint32_t>;
    using uint64 = Integer<uint64_t>;
}