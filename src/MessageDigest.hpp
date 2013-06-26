
#ifndef MESSAGEDIGEST_HPP
#define	MESSAGEDIGEST_HPP

#include "HashFunction.hpp"

class MessageDigest : public HashFunction
{
public:
   virtual ~MessageDigest() {}
   
   virtual const BytesContainer encode(const BytesContainer &data) = 0;
};

class MD2 : public MessageDigest
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
   
private:
   static const BytesContainer digits_of_pi;
   BytesContainer checksum = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
   
   void process(const BytesContainer &data, BytesContainer &hash, const uint64_t &index);
   const BytesContainer appendPadding(const BytesContainer &data) const;
};

class MD4 : public MessageDigest
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
   
private:
   static const BytesContainer left_rotation_table;
   static const BytesContainer word_indexes;
   
   static uint32_t F(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & y) | (~x & z);
   }
   
   static uint32_t G(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & y) | (x & z) | (y & z);
   }
   
   static uint32_t H(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return x ^ y ^ z;
   }
};

class MD5 : public MessageDigest
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
   
private:
   static const BytesContainer left_rotation_table;

   /* Constants obtained from the formula k(i) = sin(i + 1) * 2^32 for i = 0,...,63. */
   static const WordsContainer sine_magic_numbers;
   
   static uint32_t F(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & y) | (~x & z);
   }
   
   static uint32_t G(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & z) | (y & ~z);
   }
   
   static uint32_t H(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return x ^ y ^ z;
   }
   
   static uint32_t I(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return y ^ (x | ~z);
   }
};

#endif