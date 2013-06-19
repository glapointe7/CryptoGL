/*
 * Source : http://tools.ietf.org/html/rfc1321
 */
#ifndef MD5_HPP
#define	MD5_HPP

#include "HashFunction.hpp"

#include <vector>

class MD5 : public HashFunction
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;

private:
   /* Pad data following the MD5 Padding algorithm. Get a multiple of 512 bits. */
   virtual const BytesContainer appendPadding(const BytesContainer &data) const final;
   
   /* Transform a 512-bits block in 16 LITTLE ENDIAN 32-bits blocks. */
   virtual const WordsContainer getWordBlocks(const BytesContainer &, const uint64_t &) const final;
   
   /* 16 bytes read in little endian from state[0] to state[3]. */
   virtual const BytesContainer getOutput() const final;
   
   static uint32_t F(const uint32_t x, const uint32_t y, const uint32_t z);
   static uint32_t G(const uint32_t x, const uint32_t y, const uint32_t z);
   static uint32_t H(const uint32_t x, const uint32_t y, const uint32_t z);
   static uint32_t I(const uint32_t x, const uint32_t y, const uint32_t z);

   WordsContainer state = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476};
   //WordsContainer state = {0x01234567, 0x89ABCDEF, 0xFEDCBA98, 0x76543210};

   static const BytesContainer left_rotation_table;

   /* Constants obtained from the formula k(i) = sin(i + 1) * 2^32 for i = 0,...,63. */
   static const WordsContainer sine_magic_numbers;
};

#endif