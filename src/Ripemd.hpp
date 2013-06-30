
#ifndef RIPEMD_HPP
#define	RIPEMD_HPP

#include "HashFunction.hpp"
#include "LittleEndian.hpp"

class Ripemd : public HashFunction<uint32_t, LittleEndian32>
{
protected:
   typedef typename HashFunction<uint32_t, LittleEndian32>::BytesContainer BytesContainer;
   typedef typename HashFunction<uint32_t, LittleEndian32>::WordsContainer WordsContainer;
   
public:
   virtual ~Ripemd() {}
   virtual const BytesContainer encode(const BytesContainer &data) = 0;

protected:   
   inline static uint32_t F(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return x ^ y ^ z;
   }
   
   inline static uint32_t G(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & y) | (~x & z);
   }
   
   inline static uint32_t H(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x | ~y) ^ z;
   }
   
   inline static uint32_t I(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & z) | (y & ~z);
   }
   
   inline static uint32_t J(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return x ^ (y | ~z);
   }
   
   static const uint32_t magic_numbers1[5];
   static const uint32_t magic_numbers2[4];
   static const uint32_t magic_numbers_big2[5];
   static const uint8_t word_selection1[80];
   static const uint8_t word_selection2[80];
   static const uint8_t left_shift1[80];
   static const uint8_t left_shift2[80];
};

class Ripemd128 : public Ripemd
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
};

class Ripemd160 : public Ripemd
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
};

class Ripemd256 : public Ripemd
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
};

class Ripemd320 : public Ripemd
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
};
#endif