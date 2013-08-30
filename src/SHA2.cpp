#include "SHA2.hpp"

constexpr uint32_t SHA32Bits::round_constants[64];
constexpr uint64_t SHA64Bits::first_cubic_root_primes[80];

const SHA2<uint32_t>::BytesContainer
SHA32Bits::process(const BytesContainer &data, const uint8_t truncate_to)
{
   BytesContainer bytes = appendPadding(data);
   appendLength<BigEndian>(bytes, data.size() << 3);
   
   UInt32Container states(IV);
   const uint64_t bits_len = bytes.size();
   for (uint64_t i = 0; i < bits_len; i += 64)
   {
      UInt32Container words = getInputBlocks(bytes, i);
      words.resize(64);

      // Extention of the 32-bit 16 blocks in 64 blocks of 32 bits.
      extendWords(words, 64, {7, 18, 3, 19, 17, 10});

      UInt32Container hash(states);
      for (uint8_t j = 0; j < 64; ++j)
      {
         const uint32_t tmp1 = hash[7] + A(hash[4], 6, 11, 25) + ch(hash[4], hash[5], hash[6]) + round_constants[j] + words[j];
         const uint32_t tmp2 = A(hash[0], 2, 13, 22) + maj(hash[0], hash[1], hash[2]);
         swapHash(hash, tmp1, tmp2);
      }
      sumHash(states, hash);
   }

   return getOutput(truncate_to, states);
}

const SHA2<uint64_t>::BytesContainer
SHA64Bits::process(const BytesContainer &data, const uint8_t truncate_to)
{
   BytesContainer bytes = appendPadding(data);
   appendLength<BigEndian>(bytes, data.size() << 3);
   
   UInt64Container states(IV);
   const uint64_t bits_len = bytes.size();
   for (uint64_t i = 0; i < bits_len; i += 128)
   {
      UInt64Container words = getInputBlocks(bytes, i);
      words.resize(80);

      // Extention of the 32-bit 16 blocks in 80 blocks of 32 bits.
      extendWords(words, 80, {1, 8, 7, 19, 61, 6});

      UInt64Container hash(states);
      for (uint8_t j = 0; j < 80; ++j)
      {
         const uint64_t tmp1 = hash[7] + A(hash[4], 14, 18, 41) + ch(hash[4], hash[5], hash[6]) + first_cubic_root_primes[j] + words[j];
         const uint64_t tmp2 = A(hash[0], 28, 34, 39) + maj(hash[0], hash[1], hash[2]);
         swapHash(hash, tmp1, tmp2);
      }
      sumHash(states, hash);
   }

   return getOutput(truncate_to, states);
}

const SHA2<uint32_t>::BytesContainer SHA224::encode(const BytesContainer &data)
{
   return process(data, 7);
}

const SHA2<uint32_t>::BytesContainer SHA256::encode(const BytesContainer &data)
{
   return process(data, 8);
}

const SHA2<uint64_t>::BytesContainer SHA384::encode(const BytesContainer &data)
{
   return process(data, 12);
}

const SHA2<uint64_t>::BytesContainer SHA512::encode(const BytesContainer &data)
{
   return process(data, 16);
}

const SHA2<uint64_t>::BytesContainer SHA512_224::encode(const BytesContainer &data)
{
   buildIV({0x32, 0x32, 0x34});

   return process(data, 7);
}

const SHA2<uint64_t>::BytesContainer SHA512_256::encode(const BytesContainer &data)
{
   buildIV({0x32, 0x35, 0x36});

   return process(data, 8);
}

void SHA512_t::buildIV(const BytesContainer &t)
{
   IV.reserve(8);
   SHA512 *S = new SHA512();
   const UInt64Container IV_512 = S->getIV();

   UInt64Container tmp_IV;
   tmp_IV.reserve(8);
   for (uint8_t i = 0; i < 8; ++i)
   {
      tmp_IV.push_back(IV_512[i] ^ 0xa5a5a5a5a5a5a5a5);
   }

   S->setIV(tmp_IV);

   // Encode the string 'SHA-512/224' or 'SHA-512/256'.
   BytesContainer data = {0x53, 0x48, 0x41, 0x2D, 0x35, 0x31, 0x32, 0x2F};
   data.reserve(11);
   data.insert(data.end(), t.begin(), t.end());
   const BytesContainer answer = S->encode(data);
   delete S;

   // Get the new IV vector.
   for (uint8_t j = 0; j < 64; j += 8)
   {
      IV.push_back(BigEndian::toInteger(BytesContainer(answer.begin() + j, answer.begin() + j + 8)));
   }
   delete BE;
}