#include "RC2.hpp"

#include "Bits.hpp"

constexpr std::array<uint8_t, 256> RC2::pi_table;
constexpr std::array<uint8_t, 4> RC2::mixup_rotation;

void RC2::setKey(const BytesVector &key)
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
   BytesVector tmp_subkeys(key);
   tmp_subkeys.reserve(128);
   const uint8_t key_len = key.size();

   for (uint8_t i = key_len; i < 128; ++i)
   {
      tmp_subkeys[i] = pi_table[(tmp_subkeys[i - key_len] + tmp_subkeys[i - 1]) & 0xFF];
   }

   const uint8_t T1 = key_len * 8;
   const uint8_t TM = 0xFF >> (7 & -T1);
   const uint8_t T8 = (T1 + 7) / 8;

   tmp_subkeys[128 - T8] = pi_table[tmp_subkeys[128 - T8] & TM];
   for (int8_t i = 127 - T8; i >= 0; --i)
   {
      tmp_subkeys[i] = pi_table[tmp_subkeys[i + 1] ^ tmp_subkeys[i + T8]];
   }

   // Get the 16-bits subkeys in little-endian.
   subkeys.reserve(64);
   for (uint8_t i = 0; i < 128; i += 2)
   {
      subkeys.push_back(LittleEndian16::toIntegerRange(tmp_subkeys, i, i + 2));
   }
}

void RC2::mixUp(UInt16Vector &input, const uint8_t index, const uint8_t key_index) const
{
   const uint8_t i = 4 + index;
   input[index] += subkeys[key_index + index] + (input[(i - 2) % 4] & input[(i - 1) % 4]) + (input[(i - 3) % 4] & ~input[(i - 1) % 4]);
   input[index] = Bits::rotateLeft(input[index], mixup_rotation[index], 16);
}

void RC2::mash(UInt16Vector &input, const uint8_t index) const
{
   input[index] += subkeys[input[(index + 3) % 4] % 64];
}

void RC2::inverseMixUp(UInt16Vector &input, const uint8_t index, const uint8_t key_index) const
{
   const uint8_t i = 4 + index;
   input[index] = Bits::rotateRight(input[index], mixup_rotation[index], 16);
   input[index] -= subkeys[key_index + index] + (input[(i - 2) % 4] & input[(i - 1) % 4]) + (input[(i - 3) % 4] & ~input[(i - 1) % 4]);
}

void RC2::inverseMash(UInt16Vector &input, const uint8_t index) const
{
   input[index] -= subkeys[input[(index + 3) % 4] % 64];
}

UInt16Vector RC2::encodeBlock(const UInt16Vector &input)
{
   UInt16Vector encoded_block(input);
   for (uint8_t i = 0; i < rounds; ++i)
   {
      const uint8_t key_index = i * 4;
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

UInt16Vector RC2::decodeBlock(const UInt16Vector &input)
{
   UInt16Vector decoded_block(input);
   for (int8_t i = rounds - 1; i >= 0; --i)
   {
      const uint8_t key_index = i * 4;
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