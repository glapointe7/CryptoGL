/*
 * Hash SHA1 (http://www.itl.nist.gov/fipspubs/fip180-1.htm)
 */
#ifndef SHA1_HPP
#define	SHA1_HPP

#include "HashFunction.hpp"

class SHA1 : public HashFunction
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
   
private:
   virtual const BytesContainer appendPadding(const BytesContainer &bytes) const final;
   
   /* Transform a 512-bits block in 16 BIG ENDIAN uint32 blocks. */
   virtual const WordsContainer getWordBlocks(const BytesContainer &bytes, const uint64_t &block_index) const final;
   
   /* Concatenate hash[i] for i=0,1,2,3,4 to get a block of 128 bits 
      (20 blocks of 8 bits each) in BIG ENDIAN. */
   virtual const BytesContainer getOutput() const final;
   
   /* States initialized in LITTLE ENDIAN. */
   WordsContainer state = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xC3D2E1F0};
     
   static uint32_t F(const uint32_t x, const uint32_t y, const uint32_t z);
   static uint32_t G(const uint32_t x, const uint32_t y, const uint32_t z);
   static uint32_t H(const uint32_t x, const uint32_t y, const uint32_t z);
};

#endif