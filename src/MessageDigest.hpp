/*
 * Source : http://www.ietf.org/rfc/rfc1321.txt
 */
#ifndef MESSAGEDIGEST_HPP
#define MESSAGEDIGEST_HPP

#include "MerkleDamgardFunction.hpp"

#include <array>

namespace CryptoGL
{
    class MessageDigest : public MerkleDamgardFunction<uint32_t, LittleEndian32, LittleEndian64, 64>
    {
    protected:
        explicit MessageDigest(const uint8_t rounds)
            : MerkleDamgardFunction({0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476}, rounds, 16) { }

        virtual void compress(UInt32Vector &state) = 0;

        void swapHashElements(UInt32Vector &hash, const uint32_t special_value);
        
        static constexpr uint32_t F(const uint32_t x, const uint32_t y, const uint32_t z)
        {
            return (x & y) | (~x & z);
        }
        
        static constexpr uint32_t H(const uint32_t x, const uint32_t y, const uint32_t z)
        {
            return x ^ y ^ z;
        }
    };

    class MD4 : public MessageDigest
    {
    public:
        MD4() : MessageDigest(48) { }

    private:
        void compress(UInt32Vector &state) override;

        static constexpr std::array<uint8_t, 48> left_rotation_table = {{
            3, 7, 11, 19, 3, 7, 11, 19, 3, 7, 11, 19, 3, 7, 11, 19,
            3, 5, 9, 13, 3, 5, 9, 13, 3, 5, 9, 13, 3, 5, 9, 13,
            3, 9, 11, 15, 3, 9, 11, 15, 3, 9, 11, 15, 3, 9, 11, 15
        }};

        static constexpr std::array<uint8_t, 48> word_indexes = {{
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
            0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15,
            0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15
        }};

        static constexpr std::array<uint32_t, 3> k = {{0, 0x5A827999, 0x6ED9EBA1}};
        
        static constexpr uint32_t G(const uint32_t x, const uint32_t y, const uint32_t z)
        {
            return (x & y) | (x & z) | (y & z);
        }
    };

    class MD5 : public MessageDigest
    {
    public:
        MD5() : MessageDigest(64) { }

    private:
        void compress(UInt32Vector &state) override;

        static constexpr std::array<uint8_t, 64> left_rotation_table = {{
            7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
            5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
            4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
            6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
        }};

        /* Constants obtained from the formula k(i) = sin(i + 1) * 2^32 for i = 0,...,63. */
        static constexpr std::array<uint32_t, 64> sine_magic_numbers = {{
            0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
            0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
            0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
            0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
            0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
            0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
            0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
            0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
            0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
            0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
            0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
            0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
            0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
            0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
            0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
            0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
        }};
        
        static constexpr uint32_t G(const uint32_t x, const uint32_t y, const uint32_t z)
        {
            return (x & z) | (y & ~z);
        }
        
        static constexpr uint32_t I(const uint32_t x, const uint32_t y, const uint32_t z)
        {
            return y ^ (x | ~z);
        }
    };
}

#endif