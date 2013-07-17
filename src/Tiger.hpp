/*
 * Source : http://www.cs.technion.ac.il/~biham/Reports/Tiger/
 */
#ifndef TIGER_HPP
#define	TIGER_HPP

#include "HashFunction.hpp"
#include "LittleEndian.hpp"

class Tiger : public HashFunction<uint64_t, LittleEndian64>
{   
protected:
   typedef typename HashFunction<uint64_t, LittleEndian64>::BytesContainer BytesContainer;
   typedef typename HashFunction<uint64_t, LittleEndian64>::DWordsContainer DWordsContainer;
   
   Tiger() : HashFunction(64) {}
   virtual ~Tiger() {}
   virtual const BytesContainer encode(const BytesContainer &data) = 0;
   const BytesContainer appendPadding(const BytesContainer &data) const;
   
   const BytesContainer process(const BytesContainer &data, const uint8_t truncate_to);
   
   static void applyKeySchedule(DWordsContainer &words);
   void applyRound(uint64_t &a, uint64_t &b, uint64_t &c, const uint64_t &word, const uint8_t mult) const;
   void pass(uint64_t &a, uint64_t &b, uint64_t &c, const DWordsContainer &words, const uint8_t mult) const;
   
   static const uint64_t IV[3];
   static const uint64_t sbox[4][256];
};

class Tiger128 : public Tiger
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
};

class Tiger160 : public Tiger
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
};

class Tiger192 : public Tiger
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
};

#endif