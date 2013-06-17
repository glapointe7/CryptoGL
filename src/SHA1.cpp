
#include "SHA1.hpp"

#include "Tools.hpp"

uint32_t SHA1::F(uint32_t x, uint32_t y, uint32_t z)
{
   return (x & y) | (~x & z);
}

uint32_t SHA1::G(uint32_t x, uint32_t y, uint32_t z)
{
   return (x & y) | (x & z) | (y & z);
}

uint32_t SHA1::H(uint32_t x, uint32_t y, uint32_t z)
{
   return x ^ y ^ z;
}

const HashFunction::BitsContainer SHA1::addPadding(const BitsContainer &bits) const
{
   const uint64_t bits_len = bits.size();
   BitsContainer bits_pad(bits);
   bits_pad.reserve(bits_len + 576);

   // Ajout du bit 1 à la fin du message (bits).
   bits_pad.push_back(1);

   // Ajouter à la fin du message des bits = 0 jusqu'à une longueur de 448 (mod 512). '960 = 512 + 448'
   const uint32_t bits_pad_len = (960 - (bits_pad.size() & 0x1FF)) & 0x1FF;
   bits_pad.insert(bits_pad.end(), bits_pad_len, 0);

   // On ajoute les 64 bits de l'entier représentant la longueur initiale de 'bits'.
   for (char i = 63; i >= 0; --i)
   {
      bits_pad.push_back((bits_len >> i) & 0x1);
   }

   return bits_pad;
}

const HashFunction::WordsContainer
SHA1::getInput(const BitsContainer &bits, const uint32_t block_index) const
{
   WordsContainer words;
   words.reserve(80);

   for (uint16_t k = 0; k < 512; k += 32)
   {
      uint32_t word = 0;
      for (uint8_t j = 0; j < 32; ++j)
      {
         word |= (bits[j + k + block_index] << (31 - j));
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
      for (int8_t i = 3; i >= 0; --i)
      {
         output.push_back((state[j] >> (i << 3)) & 0xFF);
      }
   }
   
   return output;
}

const HashFunction::BytesContainer SHA1::encode(const BytesContainer &data)
{
   BitsContainer bits(addPadding(getBitsFromData(data)));
   const uint32_t bits_len = bits.size();

   for (uint32_t i = 0; i < bits_len; i += 512)
   {
      WordsContainer words = getInput(bits, i);
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