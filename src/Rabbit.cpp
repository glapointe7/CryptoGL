#include "Rabbit.hpp"

#include "Bits.hpp"
#include "exceptions/BadKeyLength.hpp"
#include "BigEndian.hpp"
#include "LittleEndian.hpp"

void Rabbit::setKey(const BytesVector &key)
{
   if (key.size() != 16)
   {
      throw BadKeyLength("Your key has to be 16 bytes length.", key.size());
   }

   this->key = key;
}

// The IV is not mandatory.
void Rabbit::setIV(const BytesVector &IV)
{
   if (IV.size() != 8)
   {
      throw BadIVLength("Your IV has to be 8 bytes length.", IV.size());
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
   constexpr uint32_t A[8] = {0x4D34D34D, 0xD34D34D3, 0x34D34D34, 0x4D34D34D,
      0xD34D34D3, 0x34D34D34, 0x4D34D34D, 0xD34D34D3
   };

   const uint32_t old_counters[8] = {counters[0], counters[1], counters[2], counters[3],
      counters[4], counters[5], counters[6], counters[7]
   };

   // Calculate new counter values.
   counters[0] += A[0] + counter_carry_bit;
   for(uint8_t i = 1; i < 8; ++i)
   {
      counters[i] += A[i] + (counters[i-1] < old_counters[i-1]);
   }
   counter_carry_bit = (counters[7] < old_counters[7]);

   uint32_t G[8];
   for (uint8_t i = 0; i < 8; ++i)
   {
      G[i] = g(states[i] + counters[i]);
   }

   // Calculate new state values.
   for(uint8_t i = 0; i < 8; i += 2)
   {
      states[i] = G[i] + Bits::rotateLeft(G[(i+7) & 7], 16) + Bits::rotateLeft(G[(i+6) & 7], 16);
      states[i + 1] = G[i + 1] + Bits::rotateLeft(G[i], 8) + G[(i+7) & 7];
   }
}

void Rabbit::generateSubkeys()
{
   // Build 4 sub-keys of 4 bytes length.
   UInt32Vector subkeys;
   subkeys.reserve(4);
   for (uint8_t i = 0; i < 16; i += 4)
   {
      subkeys.push_back(BigEndian32::toInteger(BytesVector(key.begin() + i, key.begin() + i + 4)));
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
   counters[0] = Bits::rotateLeft(subkeys[2], 16);
   counters[2] = Bits::rotateLeft(subkeys[3], 16);
   counters[4] = Bits::rotateLeft(subkeys[0], 16);
   counters[6] = Bits::rotateLeft(subkeys[1], 16);
   counters[1] = (subkeys[0] & 0xFFFF0000) | (subkeys[1] & 0xFFFF);
   counters[3] = (subkeys[1] & 0xFFFF0000) | (subkeys[2] & 0xFFFF);
   counters[5] = (subkeys[2] & 0xFFFF0000) | (subkeys[3] & 0xFFFF);
   counters[7] = (subkeys[3] & 0xFFFF0000) | (subkeys[0] & 0xFFFF);

   counter_carry_bit = 0;

   for (uint8_t i = 0; i < 4; i++)
   {
      nextState();
   }

   for (uint8_t j = 0; j < 8; ++j)
   {
      counters[j] ^= states[(j + 4) & 0x07];
   }
}

void Rabbit::IVSetup()
{
   // Generate 4 sub-IVs of 16 bits big-endian.
   UInt16Vector subIV;
   subIV.reserve(4);
   for (uint8_t i = 0; i < 8; i += 2)
   {
      subIV.push_back(BigEndian16::toInteger(BytesVector(IV.begin() + i, IV.begin() + i + 2)));
   }

   /* Modify counter values */
   for(uint8_t i = 0; i < 8; ++i)
   {
      counters[i] ^= subIV[i & 3];
   }

   for (uint8_t i = 0; i < 4; i++)
   {
      nextState();
   }
}

const BytesVector Rabbit::encode(const BytesVector &clear_text)
{
   // Clear_text have to be a multiple of 16 bytes.
   const uint64_t clear_len = clear_text.size();
   if (clear_len & 0xF)
   {
      throw BadDataLength("The size of the message has to be a multiple of 16 bytes.", clear_len);
   }

   generateSubkeys();
   if (!IV.empty())
   {
      IVSetup();
   }

   BytesVector crypted;
   crypted.reserve(clear_len);

   for (uint64_t i = 0; i < clear_len; i += 16)
   {
      nextState();

      /* Encrypt 16 bytes of data */
      const uint32_t X[4] = {
         states[0] ^ (states[5] >> 16) ^ (states[3] << 16),
         states[2] ^ (states[7] >> 16) ^ (states[5] << 16),
         states[4] ^ (states[1] >> 16) ^ (states[7] << 16),
         states[6] ^ (states[3] >> 16) ^ (states[1] << 16)
      };

      BytesVector output;
      output.reserve(16);
      for (uint8_t j = 0; j < 4; ++j)
      {
         const BytesVector tmp = LittleEndian32::toBytesVector(X[j]);
         output.insert(output.end(), tmp.begin(), tmp.end());
      }

      for (uint8_t j = 0; j < 16; ++j)
      {
         crypted.push_back(output[j] ^ clear_text[i + j]);
      }
   }

   return crypted;
}