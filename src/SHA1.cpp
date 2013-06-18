
#include "SHA1.hpp"

#include "Tools.hpp"

uint32_t SHA1::F(const uint32_t x, const uint32_t y, const uint32_t z)
{
   return (x & y) | (~x & z);
}

uint32_t SHA1::G(const uint32_t x, const uint32_t y, const uint32_t z)
{
   return (x & y) | (x & z) | (y & z);
}

uint32_t SHA1::H(const uint32_t x, const uint32_t y, const uint32_t z)
{
   return x ^ y ^ z;
}

const HashFunction::BytesContainer SHA1::appendPadding(const BytesContainer &bytes) const
{
   const uint64_t bytes_len = bytes.size();
   BytesContainer bytes_pad(bytes);
   bytes_pad.reserve(bytes_len + 64);

   // Append bit '1' at the end.
   bytes_pad.push_back(0x80);

   // Ajouter à la fin du message des bits = 0 jusqu'à une longueur de 448 (mod 512). '120 = 64 + 56'
   const uint8_t bits_pad_len = (120 - (bytes_pad.size() & 0x3F)) & 0x3F;
   bytes_pad.insert(bytes_pad.end(), bits_pad_len, 0);

   // On ajoute les 64 bits de l'entier représentant la longueur initiale de 'bits'.
   for (int8_t i = 56; i >= 0; i -= 8)
   {
      bytes_pad.push_back(((bytes_len << 3) >> i) & 0xFF);
   }

   return bytes_pad;
}

const HashFunction::WordsContainer
SHA1::getWordBlocks(const BytesContainer &bytes, const uint64_t &block_index) const
{
   WordsContainer words;
   words.reserve(80);

   for (uint8_t k = 0; k < 64; k += 4)
   {
      uint32_t word = 0;
      for (int8_t j = 3; j >= 0; --j)
      {
         word |= (bytes[3 - j + k + block_index] << (j << 3));
      }
      words.push_back(word);
   }

   // Extention of the 32-bits 16 blocks in 80 blocks of 32 bits.
   for (uint8_t j = 16; j < 80; ++j)
   {
      words.push_back(static_cast<uint32_t>(rotateLeft(words[j - 3] ^ words[j - 8] ^ words[j - 14] ^ words[j - 16], 1, 32)));
   }
   
   return words;
}

const HashFunction::BytesContainer SHA1::getOutput() const
{
   BytesContainer output;
   output.reserve(20);
   for (uint8_t j = 0; j < 5; ++j)
   {
      for (int8_t i = 24; i >= 0; i -= 8)
      {
         output.push_back((state[j] >> i) & 0xFF);
      }
   }
   
   return output;
}

const HashFunction::BytesContainer SHA1::encode(const BytesContainer &data)
{
   BytesContainer bits(appendPadding(data));
   const uint64_t bits_len = bits.size();

   for (uint64_t i = 0; i < bits_len; i += 64)
   {
      WordsContainer words = getWordBlocks(bits, i);
      WordsContainer hash(state);
      uint32_t f, k;
      for (uint8_t j = 0; j < 80; ++j)
      {
         if (j < 20)
         {
            f = F(hash[1], hash[2], hash[3]);
            k = 0x5A827999;
         }
         else if (j < 40)
         {
            f = H(hash[1], hash[2], hash[3]);
            k = 0x6ED9EBA1;
         }
         else if (j < 60)
         {
            f = G(hash[1], hash[2], hash[3]);
            k = 0x8F1BBCDC;
         }
         else
         {
            f = H(hash[1], hash[2], hash[3]);
            k = 0xCA62C1D6;
         }

         const uint32_t tmp = rotateLeft(hash[0], 5, 32) + f + hash[4] + k + words[j];
         hash[4] = hash[3];
         hash[3] = hash[2];
         hash[2] = rotateLeft(hash[1], 30, 32);
         hash[1] = hash[0];
         hash[0] = tmp;
      }

      for (uint8_t j = 0; j < 5; ++j)
      {
         state[j] += hash[j];
      }
   }

   return getOutput();
}