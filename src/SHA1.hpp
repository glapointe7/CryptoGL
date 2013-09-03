
#ifndef SHA1_HPP
#define SHA1_HPP

#include "HashFunction.hpp"
#include "BigEndian.hpp"

#define INPUT_BLOCK_LENGTH 64

class SHA1 : public HashFunction<uint32_t, BigEndian32>
{
public:
   SHA1() : HashFunction<uint32_t, BigEndian32>(INPUT_BLOCK_LENGTH, 20) {}
   virtual const BytesVector encode(const BytesVector &data) final;
   
private:
   void compress(UInt32Vector &int_block, UInt32Vector &state);
   
   static constexpr uint32_t IV[5] = {
      0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xC3D2E1F0
   };
   
   static constexpr uint32_t ch(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & y) ^ (~x & z);
   }
   
   static constexpr uint32_t G(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & y) | (x & z) | (y & z);
   }

   static constexpr uint32_t H(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return x ^ y ^ z;
   }
   
   static constexpr uint8_t rounds = 80;
};

#endif