
#ifndef RIPEMD160_HPP
#define	RIPEMD160_HPP

#include "HashFunction.hpp"

class Ripemd160 : public HashFunction
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
   static uint32_t J(const uint32_t x, const uint32_t y, const uint32_t z);
   
   WordsContainer state = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
   
   static const WordsContainer magic_numbers1;
   static const WordsContainer magic_numbers2;
   static const BytesContainer word_selection1;
   static const BytesContainer word_selection2;
   static const BytesContainer left_shift1;
   static const BytesContainer left_shift2;
};

#endif