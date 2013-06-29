
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
   static uint32_t F(const uint32_t x, const uint32_t y, const uint32_t z);
   static uint32_t G(const uint32_t x, const uint32_t y, const uint32_t z);
   static uint32_t H(const uint32_t x, const uint32_t y, const uint32_t z);
   static uint32_t I(const uint32_t x, const uint32_t y, const uint32_t z);
   static uint32_t J(const uint32_t x, const uint32_t y, const uint32_t z);
   
   static const WordsContainer magic_numbers1;
   static const WordsContainer magic_numbers2;
   static const WordsContainer magic_numbers_big2;
   static const BytesContainer word_selection1;
   static const BytesContainer word_selection2;
   static const BytesContainer left_shift1;
   static const BytesContainer left_shift2;
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