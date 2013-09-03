#include "MD2.hpp"

constexpr uint8_t MD2::digits_of_pi[];

const BytesVector MD2::appendPadding(const BytesVector &data) const
{
   const uint64_t bytes_len = data.size();
   BytesVector bytes_pad(data);

   // Pad with the value 'bytes_to_pad' at the end of bits_pad until the length is multiple of 16.
   uint8_t bytes_to_pad = 16 - (bytes_pad.size() & 0xF);
   bytes_pad.reserve(bytes_len + 16 + bytes_to_pad);
   bytes_pad.insert(bytes_pad.end(), bytes_to_pad, bytes_to_pad);

   return bytes_pad;
}

void MD2::process(const BytesVector &data, BytesVector &hash)
{
   for (uint8_t j = 0; j < 16; ++j)
   {
      hash[16 + j] = data[j];
      hash[32 + j] = hash[j] ^ data[j];
   }

   // Process 18 rounds.
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
   for (uint8_t j = 0; j < block_size; ++j)
   {
      t = checksum[j] ^= digits_of_pi[int_block[j] ^ t];
   }
}

const BytesVector MD2::encode(const BytesVector &data)
{
   const BytesVector bytes = appendPadding(data);
   BytesVector hash(48, 0);

   const uint64_t bytes_len = bytes.size();
   for (uint64_t i = 0; i < bytes_len; i += block_size)
   {
      BytesVector int_block = getInputBlocks(bytes, i);
      compress(int_block, hash);
   }

   // Last update of checksum.
   process(checksum, hash);

   return getOutput(hash);
}