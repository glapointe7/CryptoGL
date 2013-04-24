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

SHA1::BitsContainer SHA1::addPadding(const BitsContainer &bits)
{
   const uint64_t bits_len = bits.size();
   BitsContainer bits_pad(bits);
   bits_pad.reserve(bits_len + 576);

   // Ajout du bit 1 à la fin du message (bits).
   bits_pad.push_back(1);

   // Ajouter à la fin du message des bits = 0 jusqu'à une longueur de 448 (mod 512).
   const uint32_t bits_pad_len = (512 + (448 - (bits_pad.size() & 0x1FF))) & 0x1FF;
   bits_pad.insert(bits_pad.end(), bits_pad_len, 0);

   // On ajoute les 64 bits de l'entier représentant la longueur initiale de 'bits'.
   // Ajout en BIG ENDIAN
   for (char i = 63; i >= 0; --i)
   {
      bits_pad.push_back((bits_len >> i) & 0x1);
   }

   return bits_pad;
}

SHA1::BytesContainer SHA1::encode(const BytesContainer &data)
{
   BitsContainer bits(addPadding(getBitsFromData(data)));
   const uint32_t bits_len = bits.size();
   WordsContainer hash;

   for (uint32_t i = 0; i < bits_len; i += 512)
   {
      // Transformer un bloc de 512 bits en 16 blocs uint32 en BIG ENDIAN.
      WordsContainer words(80, 0);
      uint8_t n = 0;
      for (uint16_t k = 0; k < 512; k += 32)
      {
         for (uint8_t j = 0; j < 32; ++j)
         {
            words[n] |= (bits[j + k + i] << (31 - j));
         }
         n++;
      }

      // Extention des 16 blocs de 32 bits en 80 blocs de 32 bits.
      for (uint8_t j = 16; j < 80; ++j)
      {
         words[j] = (uint32_t) rotateLeft(words[j - 3] ^ words[j - 8] ^ words[j - 14] ^ words[j - 16], 1, 32);
      }

      hash = state;
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

         uint32_t tmp = rotateLeft(hash[0], 5, 32) + f + hash[4] + k + words[j];
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

   // On concatène hash[i] pour i=0,1,2,3,4 pour avoir un bloc de 128 bits (20 blocs de 8 bits chaque).
   BytesContainer crypted(20, 0);
   for (uint8_t j = 0; j < 5; ++j)
   {
      for (uint8_t i = 0; i < 4; ++i)
      {
         crypted[(j << 2) + i] = (state[j] >> ((3 - i) << 3)) & 0xFF;
      }
   }

   return crypted;
}