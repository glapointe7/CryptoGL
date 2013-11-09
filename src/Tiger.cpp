#include "Tiger.hpp"

constexpr std::array<std::array<uint64_t, 256>, 4> Tiger::sbox;

void Tiger::applyKeySchedule(UInt64Vector &words)
{
   words[0] -= words[7] ^ 0xA5A5A5A5A5A5A5A5;
   words[1] ^= words[0];
   words[2] += words[1];
   words[3] -= words[2] ^ ((~words[1]) << 19);
   words[4] ^= words[3];
   words[5] += words[4];
   words[6] -= words[5] ^ ((~words[4]) >> 23);
   words[7] ^= words[6];
   words[0] += words[7];
   words[1] -= words[0] ^ ((~words[7]) << 19);
   words[2] ^= words[1];
   words[3] += words[2];
   words[4] -= words[3] ^ ((~words[2]) >> 23);
   words[5] ^= words[4];
   words[6] += words[5];
   words[7] -= words[6] ^ 0x0123456789ABCDEF;
}

void Tiger::applyRound(uint64_t &a, uint64_t &b, uint64_t &c, const uint64_t &word, const uint8_t mult)
{
   c ^= word;
   a -= sbox[0][c & 0xFF] ^ sbox[1][(c >> 16) & 0xFF] ^
           sbox[2][(c >> 32) & 0xFF] ^ sbox[3][(c >> 48) & 0xFF];
   b += sbox[3][(c >> 8) & 0xFF] ^ sbox[2][(c >> 24) & 0xFF] ^
           sbox[1][(c >> 40) & 0xFF] ^ sbox[0][(c >> 56) & 0xFF];
   b *= mult;
}

void Tiger::pass(uint64_t &a, uint64_t &b, uint64_t &c, const UInt64Vector &words, const uint8_t mult)
{
   applyRound(a, b, c, words[0], mult);
   applyRound(b, c, a, words[1], mult);
   applyRound(c, a, b, words[2], mult);
   applyRound(a, b, c, words[3], mult);
   applyRound(b, c, a, words[4], mult);
   applyRound(c, a, b, words[5], mult);
   applyRound(a, b, c, words[6], mult);
   applyRound(b, c, a, words[7], mult);
}

void Tiger::compress(UInt64Vector &int_block, UInt64Vector &state)
{
   UInt64Vector hash(state);
   const UInt64Vector saved(hash);
   
   pass(hash[0], hash[1], hash[2], int_block, 5);
   applyKeySchedule(int_block);
   pass(hash[2], hash[0], hash[1], int_block, 7);
   applyKeySchedule(int_block);
   pass(hash[1], hash[2], hash[0], int_block, 9);

   hash[0] ^= saved[0];
   hash[1] -= saved[1];
   hash[2] += saved[2];
   
   state = hash;
}

// Specific output for Tiger-160 which is 20 bytes output and it's not a multiple of 8.
const BytesVector Tiger::getOutput(const UInt64Vector &hash) const
{
   BytesVector output;
   output.reserve(output_size);
     
   const uint8_t out_data_size = output_size >> 3;
   for (uint8_t j = 0; j < out_data_size; ++j)
   {
      const BytesVector bytes = LittleEndian64::toBytesVector(hash[j]);
      output.insert(output.end(), bytes.begin(), bytes.end());
   }

   if (output_size & 7)
   {
      const BytesVector bytes = LittleEndian64::toBytesVector(hash[out_data_size]);
      output.insert(output.end(), bytes.begin(), bytes.begin() + 4);
   }

   return output;
}