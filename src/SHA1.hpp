
#ifndef SHA1_HPP
#define	SHA1_HPP

#include "HashFunction.hpp"
#include "BigEndian.hpp"

#define INPUT_BLOCK_LENGTH 64

class SHA1 : public HashFunction<uint32_t, BigEndian<uint32_t> >
{
   typedef typename HashFunction<uint32_t, BigEndian<uint32_t> >::BytesContainer BytesContainer;
   
public:
   SHA1() : HashFunction<uint32_t, BigEndian<uint32_t> >(INPUT_BLOCK_LENGTH) {}
   virtual const BytesContainer encode(const BytesContainer &data) final;
   
private:
   static const uint32_t IV[5];
   
   inline static uint32_t ch(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & y) ^ (~x & z);
   }
   
   inline static uint32_t G(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & y) | (x & z) | (y & z);
   }

   inline static uint32_t H(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return x ^ y ^ z;
   }
};

#endif