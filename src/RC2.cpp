#include "RC2.hpp"

#include "Tools.hpp"

constexpr uint8_t RC2::pi_table[256];
constexpr uint8_t RC2::mixup_rotation[4];

void RC2::setKey(const BytesContainer &key)
{
   const uint8_t key_len = key.size();
   if (key_len > 16 || key_len < 1)
   {
      throw BadKeyLength("Your key has to be between 1 and 16 bytes length.", key_len);
   }

   this->key = key;
}

void RC2::generateSubkeys()
{
   uint8_t tmp_subkeys[128];
   const uint8_t key_len = key.size();
   for (uint8_t i = 0; i < key_len; ++i)
   {
      tmp_subkeys[i] = key[i];
   }

   for (uint8_t i = key_len; i < 128; ++i)
   {
      tmp_subkeys[i] = pi_table[(tmp_subkeys[i - key_len] + tmp_subkeys[i - 1]) & 0xFF];
   }

   const uint8_t T1 = key_len << 3;
   const uint8_t TM = 0xFF >> (7 & -T1);
   const uint8_t T8 = (T1 + 7) >> 3;

   tmp_subkeys[128 - T8] = pi_table[tmp_subkeys[128 - T8] & TM];
   for (int8_t i = 127 - T8; i >= 0; --i)
   {
      tmp_subkeys[i] = pi_table[tmp_subkeys[i + 1] ^ tmp_subkeys[i + T8]];
   }

   // Get the 16-bits subkeys in little-endian.
   subkeys.reserve(64);
   for (uint8_t i = 0; i < 64; ++i)
   {
      subkeys.push_back(tmp_subkeys[i << 1] | (tmp_subkeys[(i << 1) + 1] << 8));
   }
}

void RC2::mixUp(UInt16Container &input, const uint8_t index, const uint8_t key_index) const
{
   const uint8_t i = 4 + index;
   input[index] += subkeys[key_index + index] + (input[(i - 2) & 3] & input[(i - 1) & 3]) + (input[(i - 3) & 3] & ~input[(i - 1) & 3]);
   input[index] = rotateLeft(input[index], mixup_rotation[index], 16);
}

void RC2::mash(UInt16Container &input, const uint8_t index) const
{
   input[index] += subkeys[input[(index + 3) & 3] & 0x3F];
}

void RC2::inverseMixUp(UInt16Container &input, const uint8_t index, const uint8_t key_index) const
{
   const uint8_t i = 4 + index;
   input[index] = rotateRight(input[index], mixup_rotation[index], 16);
   input[index] -= subkeys[key_index + index] + (input[(i - 2) & 3] & input[(i - 1) & 3]) + (input[(i - 3) & 3] & ~input[(i - 1) & 3]);
}

void RC2::inverseMash(UInt16Container &input, const uint8_t index) const
{
   input[index] -= subkeys[input[(index + 3) & 3] & 0x3F];
}

const RC2::UInt16Container RC2::getIntegersFromInputBlock(const BytesContainer &block) const
{
   UInt16Container int_block(4, 0);
   for (uint8_t i = 0; i < 8; i += 2)
   {
      int_block[i >> 1] = block[i] | (block[i + 1] << 8);
   }
   
   return int_block;
}

const RC2::UInt16Container RC2::encodeBlock(const UInt16Container &input)
{
   UInt16Container encoded_block(input);
   for (uint8_t i = 0; i < 16; ++i)
   {
      const uint8_t key_index = i << 2;
      for (uint8_t j = 0; j < 4; ++j)
      {
         mixUp(encoded_block, j, key_index);
      }

      if (i == 4 || i == 10)
      {
         for (uint8_t j = 0; j < 4; ++j)
         {
            mash(encoded_block, j);
         }
      }
   }
   
   return encoded_block;
}

const RC2::UInt16Container RC2::decodeBlock(const UInt16Container &input)
{
   UInt16Container decoded_block(input);
   for (int8_t i = 15; i >= 0; --i)
   {
      const uint8_t key_index = i << 2;
      if (i == 4 || i == 10)
      {
         for (int8_t j = 3; j >= 0; --j)
         {
            inverseMash(decoded_block, j);
         }
      }

      for (int8_t j = 3; j >= 0; --j)
      {
         inverseMixUp(decoded_block, j, key_index);
      }
   }
   
   return decoded_block;
}

const RC2::BytesContainer RC2::getOutputBlock(const UInt16Container &int_block)
{
   BytesContainer output_block;
   output_block.reserve(8);
   for (uint8_t i = 0; i < 4; ++i)
   {
      output_block.push_back(int_block[i] & 0xFF);
      output_block.push_back(int_block[i] >> 8);
   }

   return output_block;
}