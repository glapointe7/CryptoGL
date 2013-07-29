/*
 * Source :
 * IV is a 512-bit length of 0.
 */
#ifndef WHIRLPOOL_HPP
#define WHIRLPOOL_HPP

#include "HashFunction.hpp"
#include "BigEndian.hpp"

#define INPUT_BLOCK_LENGTH 64

class Whirlpool : public HashFunction<uint64_t, BigEndian64>
{
private:
   typedef typename HashFunction<uint64_t, BigEndian64>::BytesContainer BytesContainer;
   typedef typename HashFunction<uint64_t, BigEndian64>::DWordsContainer DWordsContainer;

public:

   Whirlpool() : HashFunction(INPUT_BLOCK_LENGTH) {}

   virtual const BytesContainer encode(const BytesContainer &data) final;

private:
   uint64_t applyGammaPiTheta(DWordsContainer &key, const uint8_t index) const;
   void compress(const DWordsContainer &block, DWordsContainer &state);

   /* The 64-bit lookup tables. */
   static const uint64_t sbox[8][256];

   /* Round constants. */
   static constexpr uint64_t RC[10] = {
      0x1823c6e887b8014f,
      0x36a6d2f5796f9152,
      0x60bc9b8ea30c7b35,
      0x1de0d7c22e4bfe57,
      0x157737e59ff04ada,
      0x58c9290ab1a06b85,
      0xbd5d10f4cb3e0567,
      0xe427418ba77d95d8,
      0xfbee7c66dd17479e,
      0xca2dbf07ad5a8333
   };

   static constexpr uint8_t number_of_rounds = 10;

   static constexpr uint64_t IV[8] = {0, 0, 0, 0, 0, 0, 0, 0};
};

#endif