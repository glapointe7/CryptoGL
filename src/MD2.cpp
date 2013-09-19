#include "MD2.hpp"

#include "Padding.hpp"

constexpr uint8_t MD2::digits_of_pi[];

void MD2::process(const BytesVector &data, BytesVector &hash)
{
   for (uint8_t j = 0; j < input_block_size; ++j)
   {
      hash[16 + j] = data[j];
      hash[32 + j] = hash[j] ^ data[j];
   }

   // Process 864 rounds.
   uint8_t t = 0;
   for (uint8_t j = 0; j < 18; ++j)
   {
      for (uint8_t k = 0; k < 48; ++k)
      {
         t = hash[k] ^= digits_of_pi[t];
      }
      t = (t + j) & 0xFF;
   }
}

void MD2::compress(BytesVector &int_block, BytesVector &state)
{
   process(int_block, state);

   // Update checksum.
   uint8_t t = checksum[15];
   for (uint8_t j = 0; j < input_block_size; ++j)
   {
      t = checksum[j] ^= digits_of_pi[int_block[j] ^ t];
   }
}

const BytesVector MD2::encode(const BytesVector &data)
{
   const BytesVector bytes = Padding::remainingValue(data, 16);
   const uint64_t bytes_len = bytes.size();
   
   BytesVector hash(getIV());
   for (uint64_t i = 0; i < bytes_len; i += input_block_size)
   {
      BytesVector int_block = getInputBlocks(bytes, i);
      compress(int_block, hash);
   }

   // Last update of checksum.
   process(checksum, hash);

   return getOutput(hash);
}