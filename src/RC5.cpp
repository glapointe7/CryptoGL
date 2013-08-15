#include "RC5.hpp"

#include <algorithm>

#include "exceptions/BadKeyLength.hpp"

#include "LittleEndian.hpp"
#include "Tools.hpp"

void RC5::setKey(const BytesContainer &key)
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
   const uint8_t int_size = 4;
   const uint8_t key_len = key.size();
   const uint8_t tmp_key_len = (key_len + int_size - 1) / int_size;
   UInt32Container tmp_key;
   tmp_key.reserve(tmp_key_len);

   LittleEndian32 LE;
   for (uint8_t i = 0; i < key_len; i += int_size)
   {
      LE.toInteger(BytesContainer(key.begin() + i, key.begin() + i + int_size));
      tmp_key.push_back(LE.getValue());
      LE.resetValue();
   }

   // Initialize the expanded key table.
   const uint8_t subkeys_len = (rounds + 1) << 1;
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
      L = subkeys[i] = rotateLeft(subkeys[i] + L + R, 3, 32);
      R = tmp_key[j] = rotateLeft(tmp_key[j] + L + R, (L + R) & 31, 32);
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
      const uint8_t j = i << 1;
      L = rotateLeft(L ^ R, R & 31, 32) + subkeys[j];
      R = rotateLeft(R ^ L, L & 31, 32) + subkeys[j + 1];
   }
}

void RC5::decodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const
{
   for (uint8_t i = rounds; i > 0; --i)
   {
      const uint8_t j = i << 1;
      R = rotateRight(R - subkeys[j + 1], L & 31, 32) ^ L;
      L = rotateRight(L - subkeys[j], R & 31, 32) ^ R;
   }
   R -= subkeys[1];
   L -= subkeys[0];
}

const RC5::UInt32Container RC5::getIntegersFromInputBlock(const BytesContainer &block) const
{
   UInt32Container int_block(2, 0);
   LittleEndian32 LE;
   for(uint8_t i = 0; i < 2; ++i)
   {
      LE.toInteger(BytesContainer(block.begin() + (i << 2), block.begin() + (i << 2) + 4));
      int_block[i] = LE.getValue();
      LE.resetValue();
   }
   
   return int_block;
}

const RC5::UInt32Container RC5::encodeBlock(const UInt32Container &input)
{
   UInt32Container LR(input);
   encodeFeistelRounds(LR[0], LR[1], 0);
   
   return LR;
}

const RC5::UInt32Container RC5::decodeBlock(const UInt32Container &input)
{
   UInt32Container LR(input);
   decodeFeistelRounds(LR[0], LR[1], 0);
   
   return LR;
}

const RC5::BytesContainer RC5::getOutputBlock(const UInt32Container &int_block)
{
   BytesContainer output_block;
   output_block.reserve(8);
   
   LittleEndian32 LE;
   for(uint8_t i = 0; i < 2; ++i)
   {
      LE.toBytes(int_block[i]);
      const BytesContainer tmp = LE.getBytes();
      output_block.insert(output_block.end(), tmp.begin(), tmp.end());
   }

   return output_block;
}