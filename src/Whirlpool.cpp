#include "Whirlpool.hpp"

#include <algorithm>

constexpr uint64_t Whirlpool::sbox[8][256];
constexpr uint64_t Whirlpool::RC[];

uint64_t Whirlpool::applyGammaPiTheta(UInt64Vector &key, const uint8_t index) const
{
   uint64_t tmp_key = 0;
   for (uint8_t i = 0; i < 8; ++i)
   {
      tmp_key ^= sbox[i][(key[(index - i) & 7] >> ((7 - i) << 3)) & 0xFF];
   }

   return tmp_key;
}

void Whirlpool::compress(UInt64Vector &block, UInt64Vector &state)
{
   // The last state becomes the key.
   UInt64Vector key(state);

   // Apply K^0 to the cipher state.
   UInt64Vector xored_block(block);
   for (uint8_t i = 0; i < 8; ++i)
   {
      xored_block[i] ^= key[i];
   }
   
   UInt64Vector hash(8, 0);
   UInt64Vector tmp_key(8, 0);
   for (uint8_t i = 0; i < rounds; i += 2)
   {
      // Odd rounds.
      for (uint8_t j = 0; j < 8; ++j)
      {
         tmp_key[j] = applyGammaPiTheta(key, j);
      }
      tmp_key[0] ^= RC[i];


      for (uint8_t j = 0; j < 8; ++j)
      {
         hash[j] = applyGammaPiTheta(xored_block, j) ^ tmp_key[j];
      }

      // Even rounds.
      for (uint8_t j = 0; j < 8; ++j)
      {
         key[j] = applyGammaPiTheta(tmp_key, j);
      }
      key[0] ^= RC[i + 1];

      for (uint8_t j = 0; j < 8; ++j)
      {
         xored_block[j] = applyGammaPiTheta(hash, j) ^ key[j];
      }
   }

   // Apply the Miyaguchi-Preneel hash scheme.
   for (uint8_t j = 0; j < 8; ++j)
   {
      state[j] ^= xored_block[j] ^ block[j];
   }
}

const BytesVector Whirlpool::encode(const BytesVector &data)
{
   BytesVector bytes = appendPadding(data);
   bytes = appendLength<BigEndian64>(bytes, data.size() << 3);

   UInt64Vector state(IV);
   const uint64_t padded_data_len = bytes.size();
   for (uint64_t i = 0; i < padded_data_len; i += block_size)
   {
      UInt64Vector int_block = getInputBlocks(bytes, i);
      compress(int_block, state);
   }

   return getOutput(state);
}