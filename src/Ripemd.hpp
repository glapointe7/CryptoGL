/*
 * Source : http://www.esat.kuleuven.ac.be/~bosselae/ripemd160.html
 */
#ifndef RIPEMD_HPP
#define RIPEMD_HPP

#include "MerkleDamgardFunction.hpp"

#include <array>

namespace CryptoGL
{
    class Ripemd : public MerkleDamgardFunction<uint32_t, LittleEndian32, LittleEndian64, 64>
    {
    protected:
        Ripemd(const UInt32Vector &IV, const uint8_t rounds, const uint8_t output_size)
            : MerkleDamgardFunction(IV, rounds, output_size) { }

        virtual void compress(UInt32Vector &state) override = 0;

        void process128_256(UInt32Vector &hash, const uint8_t j) const;
        void process160_320(UInt32Vector &hash, const uint8_t j) const;
        
        static constexpr uint32_t F(const uint32_t x, const uint32_t y, const uint32_t z)
        {
            return x ^ y ^ z;
        }
        
        static constexpr uint32_t G(const uint32_t x, const uint32_t y, const uint32_t z)
        {
            return (x & y) | (~x & z);
        }
        
        static constexpr uint32_t H(const uint32_t x, const uint32_t y, const uint32_t z)
        {
            return (x | ~y) ^ z;
        }
        
        static constexpr uint32_t I(const uint32_t x, const uint32_t y, const uint32_t z)
        {
            return (x & z) | (y & ~z);
        }
        
        static constexpr uint32_t J(const uint32_t x, const uint32_t y, const uint32_t z)
        {
            return x ^ (y | ~z);
        }

        static constexpr std::array<uint32_t, 5> magic_numbers1 = {{
            0, 0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xA953FD4E
        }};

        static constexpr std::array<uint32_t, 4> magic_numbers2 = {{
            0x50A28BE6, 0x5C4DD124, 0x6D703EF3, 0
        }};

        static constexpr std::array<uint32_t, 5> magic_numbers_big2 = {{
            0x50A28BE6, 0x5C4DD124, 0x6D703EF3, 0x7A6D76E9, 0
        }};

        static constexpr std::array<uint8_t, 80> word_selection1 = {{
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
            7, 4, 13, 1, 10, 6, 15, 3, 12, 0, 9, 5, 2, 14, 11, 8,
            3, 10, 14, 4, 9, 15, 8, 1, 2, 7, 0, 6, 13, 11, 5, 12,
            1, 9, 11, 10, 0, 8, 12, 4, 13, 3, 7, 15, 14, 5, 6, 2,
            4, 0, 5, 9, 7, 12, 2, 10, 14, 1, 3, 8, 11, 6, 15, 13
        }};

        static constexpr std::array<uint8_t, 80> word_selection2 = {{
            5, 14, 7, 0, 9, 2, 11, 4, 13, 6, 15, 8, 1, 10, 3, 12,
            6, 11, 3, 7, 0, 13, 5, 10, 14, 15, 8, 12, 4, 9, 1, 2,
            15, 5, 1, 3, 7, 14, 6, 9, 11, 8, 12, 2, 10, 0, 4, 13,
            8, 6, 4, 1, 3, 11, 15, 0, 5, 12, 2, 13, 9, 7, 10, 14,
            12, 15, 10, 4, 1, 5, 8, 7, 6, 2, 13, 14, 0, 3, 9, 11
        }};

        static constexpr std::array<uint8_t, 80> left_shift1 = {{
            11, 14, 15, 12, 5, 8, 7, 9, 11, 13, 14, 15, 6, 7, 9, 8,
            7, 6, 8, 13, 11, 9, 7, 15, 7, 12, 15, 9, 11, 7, 13, 12,
            11, 13, 6, 7, 14, 9, 13, 15, 14, 8, 13, 6, 5, 12, 7, 5,
            11, 12, 14, 15, 14, 15, 9, 8, 9, 14, 5, 6, 8, 6, 5, 12,
            9, 15, 5, 11, 6, 8, 13, 12, 5, 12, 13, 14, 11, 8, 5, 6
        }};

        static constexpr std::array<uint8_t, 80> left_shift2 = {{
            8, 9, 9, 11, 13, 15, 15, 5, 7, 7, 8, 11, 14, 14, 12, 6,
            9, 13, 15, 7, 12, 8, 9, 11, 7, 7, 12, 7, 6, 15, 13, 11,
            9, 7, 15, 11, 8, 6, 6, 14, 12, 13, 5, 14, 13, 13, 7, 5,
            15, 5, 8, 11, 14, 14, 6, 14, 6, 9, 12, 9, 12, 5, 15, 8,
            8, 5, 12, 9, 12, 5, 14, 6, 8, 13, 6, 5, 15, 13, 11, 11
        }};
    };

    
    class Ripemd128 : public Ripemd
    {
    public:
        Ripemd128() : Ripemd({0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476}, 64, 16) { }

    private:
        void compress(UInt32Vector &state) override;
    };

    
    class Ripemd160 : public Ripemd
    {
    public:
        Ripemd160() : Ripemd({0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0}, 80, 20) { }

    private:
        void compress(UInt32Vector &state) override;
    };

    
    class Ripemd256 : public Ripemd
    {
    public:
        Ripemd256() : Ripemd({0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476,
                              0x76543210, 0xFEDCBA98, 0x89ABCDEF, 0x01234567}, 64, 32) { }

    private:
        void compress(UInt32Vector &state) override;
    };

    
    class Ripemd320 : public Ripemd
    {
    public:
        Ripemd320() : Ripemd({0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0,
                              0x76543210, 0xFEDCBA98, 0x89ABCDEF, 0x01234567, 0x3C2D1E0F}, 80, 40) { }

    private:
        void compress(UInt32Vector &state) override;
    };
}

#endif