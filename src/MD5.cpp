#include "MD5.hpp"

#include "Tools.hpp"

uint32_t MD5::F(uint32_t x, uint32_t y, uint32_t z)
{
   return (x & y) | (~x & z);
}

uint32_t MD5::G(uint32_t x, uint32_t y, uint32_t z)
{
   return (x & z) | (y & ~z);
}

uint32_t MD5::H(uint32_t x, uint32_t y, uint32_t z)
{
   return x ^ y ^ z;
}

uint32_t MD5::I(uint32_t x, uint32_t y, uint32_t z)
{
   return y ^ (x | ~z);
}

// Remplit le message avec des bits pour obtenir un multiple de 512 bits.
const MD5::BitsContainer MD5::addPadding(const BitsContainer &bits)
{
   uint64_t bits_len = bits.size();
   BitsContainer bits_pad(bits);
   bits_pad.reserve(bits_len + 576);

   // Ajout du bit 1 à la fin du message.
   bits_pad.push_back(1);

   // Ajouter à la fin du message des bits = 0 jusqu'à une longueur de 448 (mod 512).
   const uint32_t bits_pad_len = (512 + (448 - (bits_pad.size() & 0x1FF))) & 0x1FF;
   bits_pad.insert(bits_pad.end(), bits_pad_len, 0);

   // On ajoute les 64 bits de l'entier représentant la longueur initiale de 'bits'.
   // LITTLE ENDIAN. 01100010 --> 01000110
   endianSwap64(bits_len);
   for (int8_t i = 63; i >= 0; --i)
   {
         bits_pad.push_back((bits_len >> i) & 0x1);
   }

   return bits_pad;
}

const MD5::BytesContainer MD5::encode(const BytesContainer &data)
{
   BitsContainer bits(addPadding(getBitsFromData(data)));
   uint32_t bits_len = bits.size();
   WordsContainer hash;

   for (uint32_t i = 0; i < bits_len; i += 512)
   {
      // Transformer un bloc de 512 bits en 16 blocs uint32 en LITTLE ENDIAN.
      WordsContainer words(16, 0);
      uint8_t n = 0;
      for (uint16_t k = 0; k < 512; k += 32)
      {
         for (int8_t j = 31; j >= 0; --j)
         {
            words[n] |= (bits[j + k + i] << j);
         }
         endianSwap32(words[n]);
         n++;
      }

      hash = state;
      uint32_t f, g;
      for (uint8_t j = 0; j < 64; ++j)
      {
         if (j < 16)
         {
            f = F(hash[1], hash[2], hash[3]);
            g = j;
         }
         else if (i < 32)
         {
            f = G(hash[1], hash[2], hash[3]);
            g = ((5 * j) + 1) & 0xF;
         }
         else if (i < 48)
         {
            f = H(hash[1], hash[2], hash[3]);
            g = ((3 * j) + 5) & 0xF;
         }
         else
         {
            f = I(hash[1], hash[2], hash[3]);
            g = (7 * j) & 0xF;
         }

         uint32_t tmp = hash[3];
         hash[3] = hash[2];
         hash[2] = hash[1];
         hash[1] += rotateLeft(hash[0] + f + words[g] + k[j], routine[j], 32);
         hash[0] = tmp;
      }

      for (uint8_t j = 0; j < 4; ++j)
      {
         state[j] += hash[j];
      }
   }

   // On concatène hash[i] pour i=0,1,2,3 pour avoir un bloc de 128 bits 
   // (16 blocs de 8 bits chaque) en LITTLE ENDIAN (LSB ... MSB).
   BytesContainer crypted(16, 0);
   for (uint8_t j = 0; j < 4; ++j)
   {
      for(uint8_t i = 0; i < 4; ++i)
      {
         crypted[(j << 2) + i] = (state[j] >> (i << 3)) & 0xFF;
      }
   }
   
   return crypted;
}