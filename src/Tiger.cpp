#include "Tiger.hpp"

constexpr uint64_t Tiger::IV[];
constexpr uint64_t Tiger::sbox[][256];

void Tiger::applyKeySchedule(UInt64Container &words)
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

void Tiger::applyRound(uint64_t &a, uint64_t &b, uint64_t &c, const uint64_t &word, const uint8_t mult) const
{
   c ^= word;
   a -= sbox[0][c & 0xFF] ^ sbox[1][(c >> 16) & 0xFF] ^
           sbox[2][(c >> 32) & 0xFF] ^ sbox[3][(c >> 48) & 0xFF];
   b += sbox[3][(c >> 8) & 0xFF] ^ sbox[2][(c >> 24) & 0xFF] ^
           sbox[1][(c >> 40) & 0xFF] ^ sbox[0][(c >> 56) & 0xFF];
   b *= mult;
}

void Tiger::pass(uint64_t &a, uint64_t &b, uint64_t &c, const UInt64Container &words, const uint8_t mult) const
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

const Tiger::BytesContainer Tiger::appendPadding(const BytesContainer &data) const
{
   BytesContainer bytes_pad(data);
   bytes_pad.reserve(data.size() + 128);

   // Append a bit '1' at the end => 00000001.
   bytes_pad.push_back(0x01);

   // Pad with '0' bytes at the end of bits_pad until the length is 56 (mod 64).
   const uint8_t bytes_pad_len = (120 - (bytes_pad.size() & 0x3F)) & 0x3F;
   bytes_pad.insert(bytes_pad.end(), bytes_pad_len, 0);

   return bytes_pad;
}

const Tiger::BytesContainer Tiger::encode(const BytesContainer &data)
{
   BytesContainer bytes(appendPadding(data));
   appendLength<LittleEndian64>(bytes, data.size() << 3);

   UInt64Container state(IV, IV + 3);
   const uint64_t bytes_len = bytes.size();
   for (uint64_t i = 0; i < bytes_len; i += in_block_length)
   {
      UInt64Container words = getInputBlocks(bytes, i);
      UInt64Container hash(state);

      const UInt64Container saved(hash);
      pass(hash[0], hash[1], hash[2], words, 5);
      applyKeySchedule(words);
      pass(hash[2], hash[0], hash[1], words, 7);
      applyKeySchedule(words);
      pass(hash[1], hash[2], hash[0], words, 9);
      
      hash[0] ^= saved[0];
      hash[1] -= saved[1];
      hash[2] += saved[2];
      
      state = hash;
   }
   
   return getOutput(output_size, state);
}