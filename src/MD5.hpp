
#ifndef MD5_HPP
#define	MD5_HPP

#include "HashFunction.hpp"

#include <vector>

class MD5 : public HashFunction
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;

private:
   /* Pad data_bits following the MD5 Padding algorithm. Get a multiple of 512 bits. */
   virtual const BitsContainer addPadding(const BitsContainer &data_bits) const final;

   static uint32_t F(const uint32_t x, const uint32_t y, const uint32_t z);
   static uint32_t G(const uint32_t x, const uint32_t y, const uint32_t z);
   static uint32_t H(const uint32_t x, const uint32_t y, const uint32_t z);
   static uint32_t I(const uint32_t x, const uint32_t y, const uint32_t z);

   WordsContainer state = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

   static const BytesContainer routine;

   /* Constants obtained from the formula k(i) = sin(i + 1) * 2^32 for i = 0,...,63. */
   static const WordsContainer K;
};

#endif