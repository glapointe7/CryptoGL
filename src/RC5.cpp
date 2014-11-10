#include "RC5.hpp"

#include <algorithm>

#include "exceptions/BadKeyLength.hpp"

#include "Endian.hpp"
#include "Bits.hpp"

using namespace CryptoGL;

void RC5::setKey(const BytesVector &key)
{
   const uint8_t key_len = key.size();
   if (key_len < 1 || key_len > 255)
   {
      throw BadKeyLength("Your key has to be between 1 byte and 255 bytes length.", key_len);
   }

   this->key = key;
}

void RC5::generateSubkeys()
{
   constexpr uint8_t int_size = 4;
   const uint8_t key_len = key.size();
   const uint8_t tmp_key_len = (key_len + int_size - 1) / int_size;
   
   UInt32Vector tmp_key = LittleEndian32::toIntegersVector(key);

   // Initialize the expanded key table.
   const uint8_t subkeys_len = 2 * (rounds + 1);
   subkeys.resize(subkeys_len);
   subkeys[0] = P32;
   for (uint8_t i = 1; i < subkeys_len; ++i)
   {
      subkeys[i] = subkeys[i - 1] + Q32;
   }

   // This step mixes the secret key into the expanded key 'subkeys'.
   const uint8_t k = 3 * std::max(tmp_key_len, subkeys_len);
   uint32_t L = 0, R = 0;
   for (uint8_t l = 0, i = 0, j = 0; l < k; ++l)
   {
      L = subkeys[i] = Bits::rotateLeft(subkeys[i] + L + R, 3);
      R = tmp_key[j] = Bits::rotateLeft(tmp_key[j] + L + R, (L + R) % 32);
      i = (i + 1) % subkeys_len;
      j = (j + 1) % tmp_key_len;
   }
}

void RC5::encodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const
{
   L += subkeys[0];
   R += subkeys[1];
   for (uint8_t i = 1; i <= rounds; ++i)
   {
      const uint8_t j = i * 2;
      L = Bits::rotateLeft(L ^ R, R % 32) + subkeys[j];
      R = Bits::rotateLeft(R ^ L, L % 32) + subkeys[j + 1];
   }
}

void RC5::decodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const
{
   for (uint8_t i = rounds; i > 0; --i)
   {
      const uint8_t j = i * 2;
      R = Bits::rotateRight(R - subkeys[j + 1], L % 32) ^ L;
      L = Bits::rotateRight(L - subkeys[j], R % 32) ^ R;
   }
   R -= subkeys[1];
   L -= subkeys[0];
}

UInt32Vector RC5::encodeBlock(const UInt32Vector &input)
{
   UInt32Vector LR(input);
   encodeFeistelRounds(LR[0], LR[1], 0);
   
   return LR;
}

UInt32Vector RC5::decodeBlock(const UInt32Vector &input)
{
   UInt32Vector LR(input);
   decodeFeistelRounds(LR[0], LR[1], 0);
   
   return LR;
}