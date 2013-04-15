#include "Rabbit.hpp"

// La clé doit avoir 128 bits ou 16 octets.

void Rabbit::setKey(const BytesContainer &key)
{
   this->key = key;
}

void Rabbit::setIV(const BytesContainer &IV)
{
   this->IV = IV;
}

uint32_t Rabbit::g(const uint32_t x) const
{
   uint32_t a, b;
   a = x & 0xFFFF;
   b = x >> 16;

   return ((((a * a) >> 17 + a * b) >> 15) + b * b) ^ (x * x);
}

void Rabbit::update()
{
   const UInt32Bytes A = {0x4D34D34D, 0xD34D34D3, 0x34D34D34, 0x4D34D34D,
      0xD34D34D3, 0x34D34D34, 0x4D34D34D, 0xD34D34D3};

   UInt32Bytes old(counters);

   uint64_t temp = counters[0] + A[0] + counter_carry_bit;
   counters[0] = temp >> 32;
   for (unsigned char i = 1; i < 8; ++i)
   {
      uint64_t temp = counters[i] + A[i] + (counters[i - 1] < old[i - 1]);
      counters[i] = temp >> 32;
   }
   counter_carry_bit = counters[7] < old[7];

   UInt32Bytes G(8, 0);
   for (unsigned char i = 0; i < 8; ++i)
   {
      G[i] = g(states[i] + counters[i]);
   }

   states[0] = G[0] + (G[7] << 16) + (G[6] << 16) & 0xFFFFFFFF;
   states[1] = G[1] + (G[0] << 8) + G[7] & 0xFFFFFFFF;
   states[2] = G[2] + (G[1] << 16) + (G[0] << 16) & 0xFFFFFFFF;
   states[3] = G[3] + (G[2] << 8) + G[1] & 0xFFFFFFFF;
   states[4] = G[4] + (G[3] << 16) + (G[2] << 16) & 0xFFFFFFFF;
   states[5] = G[5] + (G[4] << 8) + G[3] & 0xFFFFFFFF;
   states[6] = G[6] + (G[5] << 16) + (G[4] << 16) & 0xFFFFFFFF;
   states[7] = G[7] + (G[6] << 8) + G[5] & 0xFFFFFFFF;
}

void Rabbit::initialize()
{
   // On sépare la clé en 8 sous-clés de 16 bits (2 octets) chacun.
   UInt16Bytes subkeys;
   subkeys.reserve(8);
   unsigned char key_len = key.size();

   for (unsigned char i = 0; i < key_len; i += 2)
   {
      // On concatène 2 octets.
      subkeys.push_back((key[i] << 8) | key[i + 1]);
   }

   // Initialisation de la forme initiale de states et counters.
   for (unsigned char j = 0; j < 8; ++j)
   {
      if (j % 2 == 0)
      {
         states[j] = (subkeys[(j + 1) & 0x07] << 16) | subkeys[j];
         counters[j] = (subkeys[(j + 4) & 0x07] << 16) | subkeys[(j + 5) & 0x07];
      }
      else
      {
         states[j] = (subkeys[(j + 5) & 0x07] << 16) | subkeys[(j + 4) & 0x07];
         counters[j] = (subkeys[j] << 16) | subkeys[(j + 1) & 0x07];
      }
   }

   for (unsigned char i = 0; i < 4; i++)
   {
      update();
   }

   // Réinitialisation des counters.
   for (unsigned char j = 0; j < 8; ++j)
   {
      counters[j] ^= states[(j + 4) & 0x07];
   }
}

void Rabbit::initializeCounters()
{
   // Générer 4 sous-IV de 16 bits chacun.
   UInt16Bytes subIV;
   subIV.reserve(4);
   for (unsigned char i = 0; i < 8; i += 2)
   {
      subIV.push_back((IV[i] << 8) | IV[i + 1]);
   }


   counters[0] ^= (subIV[0] << 16) | subIV[1];
   counters[1] ^= (subIV[3] << 16) | subIV[1];
   counters[2] ^= (subIV[2] << 16) | subIV[3];
   counters[3] ^= (subIV[2] << 16) | subIV[0];

   counters[4] ^= (subIV[0] << 16) | subIV[1];
   counters[5] ^= (subIV[3] << 16) | subIV[1];
   counters[6] ^= (subIV[2] << 16) | subIV[3];
   counters[7] ^= (subIV[2] << 16) | subIV[0];

   for (unsigned char i = 0; i < 4; i++)
   {
      update();
   }
}

// Retourne un block de 16 octets (128 bits) encodé / décodé.
Rabbit::BytesContainer Rabbit::getOutputBytes()
{
   UInt32Bytes X(4, 0);
   BytesContainer out(16, 0);

   update();
   
   X[0] = states[0] ^ (states[5] >> 16 ^ states[3] << 16);
   X[1] = states[2] ^ (states[7] >> 16 ^ states[5] << 16);
   X[2] = states[4] ^ (states[1] >> 16 ^ states[7] << 16);
   X[3] = states[6] ^ (states[3] >> 16 ^ states[1] << 16);
   
   for(unsigned char i = 0; i < 4; ++i)
   {
      out[i << 2] = static_cast<unsigned char>(X[i]);
      out[(i << 2)+1] = static_cast<unsigned char>(X[i] >> 8);
      out[(i << 2)+2] = static_cast<unsigned char>(X[i] >> 16);
      out[(i << 2)+3] = static_cast<unsigned char>(X[i] >> 24);
   }
   
   return out;
}

Rabbit::BytesContainer Rabbit::encode(const BytesContainer &clear_text)
{
   initialize();
   if (!IV.empty())
   {
      initializeCounters();
   }
   
   unsigned int clear_len = clear_text.size();
   unsigned int i = 0;
   while (i < clear_len)
   {
      BytesContainer crypted(getOutputBytes());
      
      for (unsigned char j = 0; j < 16; ++j)
      {
         crypted[j] ^= clear_text[i];
         i++;
         if(i >= clear_len)
         {
            return crypted;
         }
      }
   }
   // TODO Make sure that it can NEVER reach this point
}

Rabbit::BytesContainer Rabbit::decode(const BytesContainer &cipher_text)
{
   return encode(cipher_text);
}
