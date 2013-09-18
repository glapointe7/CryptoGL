#include "SHA2.hpp"

constexpr uint32_t SHA32Bits::round_constants[];
constexpr uint64_t SHA64Bits::first_cubic_root_primes[];

void SHA32Bits::compress(UInt32Vector &int_block, UInt32Vector &state)
{
   int_block.resize(rounds);

   // Extention of the 32-bit 16 blocks in 64 blocks of 32 bits.
   extendWords(int_block, {7, 18, 3, 19, 17, 10});

   UInt32Vector hash(state);
   for (uint8_t j = 0; j < rounds; ++j)
   {
      const uint32_t tmp1 = hash[7] + A(hash[4], 6, 11, 25) + ch(hash[4], hash[5], hash[6]) + round_constants[j] + int_block[j];
      const uint32_t tmp2 = A(hash[0], 2, 13, 22) + maj(hash[0], hash[1], hash[2]);
      swapHash(hash, tmp1, tmp2);
   }
   
   finalize(state, hash);
}

void SHA64Bits::compress(UInt64Vector &int_block, UInt64Vector &state)
{
   int_block.resize(rounds);

   // Extention of the 32-bit 16 blocks in 80 blocks of 32 bits.
   extendWords(int_block, {1, 8, 7, 19, 61, 6});

   UInt64Vector hash(state);
   for (uint8_t j = 0; j < rounds; ++j)
   {
      const uint64_t tmp1 = hash[7] + A(hash[4], 14, 18, 41) + ch(hash[4], hash[5], hash[6]) + first_cubic_root_primes[j] + int_block[j];
      const uint64_t tmp2 = A(hash[0], 28, 34, 39) + maj(hash[0], hash[1], hash[2]);
      swapHash(hash, tmp1, tmp2);
   }
   
   finalize(state, hash);
}

void SHA512_t::buildIV(const BytesVector &t)
{
   SHA512 *S = new SHA512();
   const UInt64Vector IV_512 = S->getIV();

   UInt64Vector tmp_IV;
   tmp_IV.reserve(8);
   for (uint8_t i = 0; i < 8; ++i)
   {
      tmp_IV.push_back(IV_512[i] ^ 0xa5a5a5a5a5a5a5a5);
   }

   S->setIV(tmp_IV);

   // Encode the string 'SHA-512/224' or 'SHA-512/256'.
   BytesVector data = {0x53, 0x48, 0x41, 0x2D, 0x35, 0x31, 0x32, 0x2F};
   data.reserve(11);
   data.insert(data.end(), t.begin(), t.end());
   const BytesVector answer = S->encode(data);
   delete S;

   // Get the new IV vector.
   IV.clear();
   IV.reserve(8);
   for (uint8_t j = 0; j < 64; j += 8)
   {
      IV.push_back(BigEndian64::toInteger(BytesVector(answer.begin() + j, answer.begin() + j + 8)));
   }
}