#include "AES.hpp"

#include "exceptions/BadKeyLength.hpp"
#include "BigEndian.hpp"
#include "Tools.hpp"

#include <algorithm>

constexpr uint8_t AES::shift_indexes[];
constexpr uint8_t AES::inverse_shift_indexes[];
constexpr uint32_t AES::round_constants[];
constexpr uint8_t AES::sbox[];
constexpr uint8_t AES::inverse_sbox[];

constexpr uint8_t AES::table_2[];
constexpr uint8_t AES::table_3[];
constexpr uint8_t AES::table_9[];
constexpr uint8_t AES::table_11[];
constexpr uint8_t AES::table_13[];
constexpr uint8_t AES::table_14[256];

void AES::setKey(const BytesContainer &key)
{
   const uint8_t key_len = key.size();
   if (key_len != 16 && key_len != 24 && key_len != 32)
   {
      throw BadKeyLength("Your key has to be 128, 192 or 256 bits length.", key_len);
   }

   switch (key_len)
   {
      case 16: rounds = 10;
         break;
      case 24: rounds = 12;
         break;
      case 32: rounds = 14;
         break;
   }
   this->key = key;
}

void AES::subBytes(UInt32Container &state, const uint8_t *box)
{
   for (uint8_t i = 0; i < 4; ++i)
   {
      state[i] = box[(state[i] >> 24) & 0xFF] << 24
              | box[(state[i] >> 16) & 0xFF] << 16
              | box[(state[i] >> 8) & 0xFF] << 8
              | box[state[i] & 0xFF];
   }
}

void AES::shiftRows(UInt32Container &state)
{
   uint32_t state_transposed[4];
   state_transposed[0] = ((state[0] >> 24) & 0xFF) << 24 | ((state[1] >> 16) & 0xFF) << 16 
           | ((state[2] >> 8) & 0xFF) << 8 | (state[3] & 0xFF);
   
   state_transposed[1] = ((state[1] >> 24) & 0xFF) << 24 | ((state[2] >> 16) & 0xFF) << 16 
           | ((state[3] >> 8) & 0xFF) << 8 | (state[0] & 0xFF);
   
   state_transposed[2] = ((state[2] >> 24) & 0xFF) << 24 | ((state[3] >> 16) & 0xFF) << 16 
           | ((state[0] >> 8) & 0xFF) << 8 | (state[1] & 0xFF);
   
   state_transposed[3] = ((state[3] >> 24) & 0xFF) << 24 | ((state[0] >> 16) & 0xFF) << 16
           | ((state[1] >> 8) & 0xFF) << 8 | (state[2] & 0xFF);
   state = UInt32Container(state_transposed, state_transposed + 4);
   
}

void AES::inverseShiftRows(UInt32Container &state)
{
   uint32_t state_transposed[4];
   state_transposed[0] = ((state[0] >> 24) & 0xFF) << 24 | ((state[3] >> 16) & 0xFF) << 16 
           | ((state[2] >> 8) & 0xFF) << 8 | (state[1] & 0xFF);
   
   state_transposed[1] = ((state[1] >> 24) & 0xFF) << 24 | ((state[0] >> 16) & 0xFF) << 16 
           | ((state[3] >> 8) & 0xFF) << 8 | (state[2] & 0xFF);
   
   state_transposed[2] = ((state[2] >> 24) & 0xFF) << 24 | ((state[1] >> 16) & 0xFF) << 16 
           | ((state[0] >> 8) & 0xFF) << 8 | (state[3] & 0xFF);
   
   state_transposed[3] = ((state[3] >> 24) & 0xFF) << 24 | ((state[2] >> 16) & 0xFF) << 16
           | ((state[1] >> 8) & 0xFF) << 8 | (state[0] & 0xFF);
   state = UInt32Container(state_transposed, state_transposed + 4);
}

void AES::mixColumns(UInt32Container &state) const
{
   for(uint8_t i = 0; i < 4; ++i)
   {
      uint32_t x = 0;
      x |= (table_2[(state[i] >> 24) & 0xFF] ^ table_3[(state[i] >> 16) & 0xFF] 
           ^ ((state[i] >> 8) & 0xFF) ^ (state[i] & 0xFF)) << 24;
      
      x |= (((state[i] >> 24) & 0xFF) ^ table_2[(state[i] >> 16) & 0xFF] 
           ^ table_3[(state[i] >> 8) & 0xFF] ^ (state[i] & 0xFF)) << 16;
      
      x |= (((state[i] >> 24) & 0xFF) ^ ((state[i] >> 16) & 0xFF) 
           ^ table_2[(state[i] >> 8) & 0xFF] ^ table_3[state[i] & 0xFF]) << 8;
      
      x |= table_3[(state[i] >> 24) & 0xFF] ^ ((state[i] >> 16) & 0xFF) 
           ^ ((state[i] >> 8) & 0xFF) ^ table_2[state[i] & 0xFF];
      state[i] = x;
   }
}

void AES::inverseMixColumns(UInt32Container &state) const
{
   for(uint8_t i = 0; i < 4; ++i)
   {
      uint32_t x = 0;
      x |= (table_14[(state[i] >> 24) & 0xFF] ^ table_11[(state[i] >> 16) & 0xFF] 
           ^ table_13[(state[i] >> 8) & 0xFF] ^ table_9[state[i] & 0xFF]) << 24;
      
      x |= (table_9[(state[i] >> 24) & 0xFF] ^ table_14[(state[i] >> 16) & 0xFF] 
           ^ table_11[(state[i] >> 8) & 0xFF] ^ table_13[state[i] & 0xFF]) << 16;
      
      x |= (table_13[(state[i] >> 24) & 0xFF] ^ table_9[(state[i] >> 16) & 0xFF] 
           ^ table_14[(state[i] >> 8) & 0xFF] ^ table_11[state[i] & 0xFF]) << 8;
      
      x |= table_11[(state[i] >> 24) & 0xFF] ^ table_13[(state[i] >> 16) & 0xFF] 
           ^ table_9[(state[i] >> 8) & 0xFF] ^ table_14[state[i] & 0xFF];
      state[i] = x;
   }
}

uint32_t AES::subWord(const uint32_t word) const
{
   return sbox[word & 0xFF]
           | (sbox[(word >> 8) & 0xFF]) << 8
           | (sbox[(word >> 16) & 0xFF]) << 16
           | (sbox[(word >> 24) & 0xFF]) << 24;
}

void AES::generateSubkeys()
{
   const uint8_t Nk = key.size() >> 2;
   const uint8_t max_round = (rounds + 1) << 2;
   BigEndian32 BE;

   subkeys.reserve(max_round);
   for (uint8_t i = 0; i < Nk; ++i)
   {
      BE.toInteger(BytesContainer(key.begin() + (i << 2), key.begin() + ((i + 1) << 2)));
      subkeys.push_back(BE.getValue());
      BE.resetValue();
   }

   for (uint8_t i = Nk; i < max_round; ++i)
   {
      uint32_t tmp = subkeys[i - 1];
      if (!(i % Nk))
      {
         tmp = subWord(static_cast<uint32_t> (rotateLeft(tmp, 8, 32))) ^ round_constants[(i / Nk) - 1];
      }
      else if (Nk > 6 && i % Nk == 4)
      {
         tmp = subWord(tmp);
      }
      subkeys.push_back(subkeys[i - Nk] ^ tmp);
   }
}

void AES::addRoundKey(UInt32Container &state, const uint8_t round) const
{
   for (uint8_t i = 0; i < 4; ++i)
   {
      state[i] ^= subkeys[(round << 2) + i];
   }
}

const AES::UInt32Container AES::getIntegersFromInputBlock(const BytesContainer &block) const
{
   UInt32Container int_block(4, 0);
   BigEndian32 BE;
   for (uint8_t i = 0; i < 16; i += 4)
   {
      BE.toInteger(BytesContainer(block.begin() + i, block.begin() + i + 4));
      int_block[i >> 2] = BE.getValue();
      BE.resetValue();
   }
   
   return int_block;
}

const AES::UInt32Container AES::encodeBlock(const UInt32Container &input)
{
   UInt32Container encoded_block(input);
   addRoundKey(encoded_block, 0);

   for (uint8_t i = 1; i < rounds; ++i)
   {
      subBytes(encoded_block, sbox);
      shiftRows(encoded_block);
      mixColumns(encoded_block);
      addRoundKey(encoded_block, i);
   }

   // Last round : no mixColumns.
   subBytes(encoded_block, sbox);
   shiftRows(encoded_block);
   addRoundKey(encoded_block, rounds);
   
   return encoded_block;
}

const AES::UInt32Container AES::decodeBlock(const UInt32Container &input)
{
   UInt32Container decoded_block(input);
   addRoundKey(decoded_block, rounds);

   for (uint8_t i = rounds - 1; i >= 1; --i)
   {
      inverseShiftRows(decoded_block);
      subBytes(decoded_block, inverse_sbox);
      addRoundKey(decoded_block, i);
      inverseMixColumns(decoded_block);
   }

   inverseShiftRows(decoded_block);
   subBytes(decoded_block, inverse_sbox);
   addRoundKey(decoded_block, 0);
   
   return decoded_block;
}

const AES::BytesContainer AES::getOutputBlock(const UInt32Container &int_block)
{
   BytesContainer output_block;
   output_block.reserve(16);

   BigEndian32 BE;
   for(uint8_t i = 0; i < 4; ++i)
   {
      BE.toBytes(int_block[i]);
      const BytesContainer out = BE.getBytes();
      output_block.insert(output_block.end(), out.begin(), out.end());
   }

   return output_block;
}