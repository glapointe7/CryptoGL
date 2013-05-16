#include "Rabbit.hpp"

#include "Tools.hpp"
#include "exceptions/BadKeyLength.hpp"

#include <limits>

// La clé doit avoir 128 bits = 16 octets.

void Rabbit::setKey(const BytesContainer &key)
{
   if (key.size() != 16)
   {
      throw BadKeyLength("Your key have to have 16 bytes length.", key.size());
   }

   this->key = key;
}

// IV = Initial Vector et doit être de 8 octets = 64 bits.
// Le IV peut être nul.

void Rabbit::setIV(const BytesContainer &IV)
{
   if (IV.size() != 8)
   {
      throw BadIVLength("Your IV size have to be 8 bytes length.", IV.size());
   }

   this->IV = IV;
}

uint32_t Rabbit::g(const uint32_t x)
{
   // Construct high and low argument for squaring.
   const uint32_t a = x & 0xFFFF;
   const uint32_t b = x >> 16;

   // Calculate high and low result of squaring.
   const uint32_t h = ((((a * a) >> 17) + (a * b)) >> 15) + (b * b);
   const uint32_t l = x * x;

   // Return high XOR low. 
   return h ^ l;
}

void Rabbit::nextState()
{
   const UInt32Container A = {0x4D34D34D, 0xD34D34D3, 0x34D34D34, 0x4D34D34D,
      0xD34D34D3, 0x34D34D34, 0x4D34D34D, 0xD34D34D3};

   UInt32Container G;
   G.reserve(8);
   UInt32Container old_counters(counters);

   // Calculate new counter values.
   counters[0] += A[0] + counter_carry_bit;
   counters[1] += A[1] + (counters[0] < old_counters[0]);
   counters[2] += A[2] + (counters[1] < old_counters[1]);
   counters[3] += A[3] + (counters[2] < old_counters[2]);
   counters[4] += A[4] + (counters[3] < old_counters[3]);
   counters[5] += A[5] + (counters[4] < old_counters[4]);
   counters[6] += A[6] + (counters[5] < old_counters[5]);
   counters[7] += A[7] + (counters[6] < old_counters[6]);
   counter_carry_bit = (counters[7] < old_counters[7]);

   for (uint8_t i = 0; i < 8; ++i)
   {
      G.push_back(g(states[i] + counters[i]));
   }

   // Calculate new state values.
   states[0] = G[0] + rotateLeft(G[7], 16, 32) + rotateLeft(G[6], 16, 32);
   states[1] = G[1] + rotateLeft(G[0], 8, 32) + G[7];
   states[2] = G[2] + rotateLeft(G[1], 16, 32) + rotateLeft(G[0], 16, 32);
   states[3] = G[3] + rotateLeft(G[2], 8, 32) + G[1];
   states[4] = G[4] + rotateLeft(G[3], 16, 32) + rotateLeft(G[2], 16, 32);
   states[5] = G[5] + rotateLeft(G[4], 8, 32) + G[3];
   states[6] = G[6] + rotateLeft(G[5], 16, 32) + rotateLeft(G[4], 16, 32);
   states[7] = G[7] + rotateLeft(G[6], 8, 32) + G[5];
}

void Rabbit::keySetup()
{
   // Build 4 sub-keys of 4 bytes length.
   UInt32Container subkeys;
   subkeys.reserve(4);
   const unsigned char key_len = key.size();

   for (unsigned char i = 0; i < key_len; i += 4)
   {
      subkeys.push_back((key[i] << 24) | (key[i + 1] << 16) | (key[i + 2] << 8) | key[i + 3]);
   }


   /* Generate initial state variables */
   states[0] = subkeys[0];
   states[2] = subkeys[1];
   states[4] = subkeys[2];
   states[6] = subkeys[3];
   states[1] = (subkeys[3] << 16) | (subkeys[2] >> 16);
   states[3] = (subkeys[0] << 16) | (subkeys[3] >> 16);
   states[5] = (subkeys[1] << 16) | (subkeys[0] >> 16);
   states[7] = (subkeys[2] << 16) | (subkeys[1] >> 16);

   /* Generate initial counter values */
   counters[0] = rotateLeft(subkeys[2], 16, 32);
   counters[2] = rotateLeft(subkeys[3], 16, 32);
   counters[4] = rotateLeft(subkeys[0], 16, 32);
   counters[6] = rotateLeft(subkeys[1], 16, 32);
   counters[1] = (subkeys[0] & 0xFFFF0000) | (subkeys[1] & 0xFFFF);
   counters[3] = (subkeys[1] & 0xFFFF0000) | (subkeys[2] & 0xFFFF);
   counters[5] = (subkeys[2] & 0xFFFF0000) | (subkeys[3] & 0xFFFF);
   counters[7] = (subkeys[3] & 0xFFFF0000) | (subkeys[0] & 0xFFFF);

   counter_carry_bit = 0;

   for (unsigned char i = 0; i < 4; i++)
   {
      nextState();
   }

   for (unsigned char j = 0; j < 8; ++j)
   {
      counters[j] ^= states[(j + 4) & 0x07];
   }
}

void Rabbit::IVSetup()
{
   // Generate 4 sub-IVs of 16 bits.
   UInt32Container subIV;
   subIV.reserve(4);
   for (unsigned char i = 0; i < 8; i += 2)
   {
      subIV.push_back((IV[i] << 8) | IV[i + 1]);
   }

   /* Modify counter values */
   counters[0] ^= subIV[0];
   counters[1] ^= subIV[1];
   counters[2] ^= subIV[2];
   counters[3] ^= subIV[3];
   counters[4] ^= subIV[0];
   counters[5] ^= subIV[1];
   counters[6] ^= subIV[2];
   counters[7] ^= subIV[3];

   for (unsigned char i = 0; i < 4; i++)
   {
      nextState();
   }
}

// Initialize states, counters and counter_carry_bit.

void Rabbit::reset()
{
   states = {0, 0, 0, 0, 0, 0, 0, 0};
   counters = {0, 0, 0, 0, 0, 0, 0, 0};
   counter_carry_bit = 0;
}

const Rabbit::BytesContainer Rabbit::encode(const BytesContainer &clear_text)
{
   // Clear_text have to be a multiple of 16 bytes.
   const uint32_t clear_len = clear_text.size();
   if (clear_len & 0xF)
   {
      throw BadDataLength("Your data provided have to be a multiple of 16 bytes.", clear_len);
   }

   keySetup();
   if (!IV.empty())
   {
      IVSetup();
   }

   BytesContainer crypted;
   crypted.reserve(clear_len);

   for (uint32_t i = 0; i < clear_len; i += 16)
   {
      nextState();

      /* Encrypt 16 bytes of data */
      const UInt32Container X = {
         states[0] ^ (states[5] >> 16) ^ (states[3] << 16),
         states[2] ^ (states[7] >> 16) ^ (states[5] << 16),
         states[4] ^ (states[1] >> 16) ^ (states[7] << 16),
         states[6] ^ (states[3] >> 16) ^ (states[1] << 16)
      };

      BytesContainer output;
      output.reserve(16);
      for (uint8_t j = 0; j < 4; ++j)
      {
         for (uint8_t k = 0; k < 4; ++k)
         {
            output.push_back(static_cast<uint8_t> (X[j] >> (k << 3)) & 0xFF);
         }
      }

      for (uint8_t j = 0; j < 16; ++j)
      {
         crypted.push_back(output[j] ^ clear_text[i + j]);
      }
   }

   return crypted;
}

const Rabbit::BytesContainer Rabbit::decode(const BytesContainer &cipher_text)
{
   return encode(cipher_text);
}
