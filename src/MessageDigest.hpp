
#ifndef MESSAGEDIGEST_HPP
#define	MESSAGEDIGEST_HPP

#include "HashFunction.hpp"
#include "LittleEndian.hpp"

class MessageDigest : public HashFunction<uint32_t, LittleEndian32>
{
protected:
   typedef typename HashFunction<uint32_t, LittleEndian32>::BytesContainer BytesContainer;
   typedef typename HashFunction<uint32_t, LittleEndian32>::WordsContainer WordsContainer;
   
   virtual ~MessageDigest() {}
   virtual const BytesContainer encode(const BytesContainer &data) = 0;
};

class MD2 : public MessageDigest
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
   
private:
   static const uint8_t digits_of_pi[256];
   BytesContainer checksum = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
   
   void process(const BytesContainer &data, BytesContainer &hash, const uint64_t &index);
   const BytesContainer appendPadding(const BytesContainer &data) const;
};

class MD4 : public MessageDigest
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
   
private:
   static const uint8_t left_rotation_table[48];
   static const uint8_t word_indexes[48];
   
   inline static uint32_t F(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & y) | (~x & z);
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

class MD5 : public MessageDigest
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
   
private:
   static const uint8_t left_rotation_table[64];

   /* Constants obtained from the formula k(i) = sin(i + 1) * 2^32 for i = 0,...,63. */
   static const uint32_t sine_magic_numbers[64];
   
   inline static uint32_t F(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & y) | (~x & z);
   }
   
   inline static uint32_t G(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & z) | (y & ~z);
   }
   
   inline static uint32_t H(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return x ^ y ^ z;
   }
   
   inline static uint32_t I(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return y ^ (x | ~z);
   }
};

#endif