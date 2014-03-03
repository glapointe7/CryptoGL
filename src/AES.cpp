#include "AES.hpp"

#include "exceptions/BadKeyLength.hpp"
#include "Endian.hpp"
#include "Bits.hpp"

#include <algorithm>

constexpr std::array<uint8_t, 16> AES::shift_indexes;
constexpr std::array<uint8_t, 16> AES::inverse_shift_indexes;
constexpr std::array<uint32_t, 15> AES::round_constants;
constexpr std::array<uint8_t, 256> AES::sbox;
constexpr std::array<uint8_t, 256> AES::inverse_sbox;

constexpr std::array<uint8_t, 256> AES::table_2;
constexpr std::array<uint8_t, 256> AES::table_3;
constexpr std::array<uint8_t, 256> AES::table_9;
constexpr std::array<uint8_t, 256> AES::table_11;
constexpr std::array<uint8_t, 256> AES::table_13;
constexpr std::array<uint8_t, 256> AES::table_14;

void AES::setKey(const BytesVector &key)
{
   const uint8_t key_len = key.size();
   if (key_len != 16 && key_len != 24 && key_len != 32)
   {
      throw BadKeyLength("Your key has to be 128, 192 or 256 bits length.", key_len);
   }

   switch (key_len)
   {
      case 24: rounds = 12;
         break;
      case 32: rounds = 14;
         break;
   }
   this->key = key;
}

void AES::subBytes(const std::array<uint8_t, 256> &box)
{
   for (uint8_t i = 0; i < 4; ++i)
   {
      state[i] = box[(state[i] >> 24) & 0xFF] << 24
              | box[(state[i] >> 16) & 0xFF] << 16
              | box[(state[i] >> 8) & 0xFF] << 8
              | box[state[i] & 0xFF];
   }
}

void AES::shiftRows()
{
   std::array<uint32_t, 4> state_transposed;
   state_transposed[0] = ((state[0] >> 24) & 0xFF) << 24 | ((state[1] >> 16) & 0xFF) << 16 
           | ((state[2] >> 8) & 0xFF) << 8 | (state[3] & 0xFF);
   
   state_transposed[1] = ((state[1] >> 24) & 0xFF) << 24 | ((state[2] >> 16) & 0xFF) << 16 
           | ((state[3] >> 8) & 0xFF) << 8 | (state[0] & 0xFF);
   
   state_transposed[2] = ((state[2] >> 24) & 0xFF) << 24 | ((state[3] >> 16) & 0xFF) << 16 
           | ((state[0] >> 8) & 0xFF) << 8 | (state[1] & 0xFF);
   
   state_transposed[3] = ((state[3] >> 24) & 0xFF) << 24 | ((state[0] >> 16) & 0xFF) << 16
           | ((state[1] >> 8) & 0xFF) << 8 | (state[2] & 0xFF);
   state = UInt32Vector(state_transposed.begin(), state_transposed.end());
   
}

void AES::inverseShiftRows()
{
   std::array<uint32_t, 4> state_transposed;
   state_transposed[0] = ((state[0] >> 24) & 0xFF) << 24 | ((state[3] >> 16) & 0xFF) << 16 
           | ((state[2] >> 8) & 0xFF) << 8 | (state[1] & 0xFF);
   
   state_transposed[1] = ((state[1] >> 24) & 0xFF) << 24 | ((state[0] >> 16) & 0xFF) << 16 
           | ((state[3] >> 8) & 0xFF) << 8 | (state[2] & 0xFF);
   
   state_transposed[2] = ((state[2] >> 24) & 0xFF) << 24 | ((state[1] >> 16) & 0xFF) << 16 
           | ((state[0] >> 8) & 0xFF) << 8 | (state[3] & 0xFF);
   
   state_transposed[3] = ((state[3] >> 24) & 0xFF) << 24 | ((state[2] >> 16) & 0xFF) << 16
           | ((state[1] >> 8) & 0xFF) << 8 | (state[0] & 0xFF);
   state = UInt32Vector(state_transposed.begin(), state_transposed.end());
}

void AES::mixColumns()
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

void AES::inverseMixColumns()
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

uint32_t AES::subWord(const uint32_t word)
{
   return sbox[word & 0xFF]
           | (sbox[(word >> 8) & 0xFF]) << 8
           | (sbox[(word >> 16) & 0xFF]) << 16
           | (sbox[(word >> 24) & 0xFF]) << 24;
}

void AES::generateSubkeys()
{
   const uint8_t Nk = key.size() / 4;
   const uint8_t max_round = (rounds + 1) * 4;

   subkeys.reserve(max_round);
   subkeys.extend(BigEndian32::toIntegersVector(key));

   for (uint8_t i = Nk; i < max_round; ++i)
   {
      uint32_t tmp = subkeys[i - 1];
      if (!(i % Nk))
      {
         tmp = subWord(Bits::rotateLeft(tmp, 8)) ^ round_constants[(i / Nk) - 1];
      }
      else if (Nk > 6 && i % Nk == 4)
      {
         tmp = subWord(tmp);
      }
      subkeys.push_back(subkeys[i - Nk] ^ tmp);
   }
}

void AES::addRoundKey(const uint8_t round)
{
   const uint8_t j = round * 4;
   for (uint8_t i = 0; i < 4; ++i)
   {
      state[i] ^= subkeys[j + i];
   }
}

UInt32Vector AES::encodeBlock(const UInt32Vector &input)
{
   state = input;
   addRoundKey(0);

   for (uint8_t i = 1; i < rounds; ++i)
   {
      subBytes(sbox);
      shiftRows();
      mixColumns();
      addRoundKey(i);
   }

   // Last round : no mixColumns.
   subBytes(sbox);
   shiftRows();
   addRoundKey(rounds);
   
   return state;
}

UInt32Vector AES::decodeBlock(const UInt32Vector &input)
{
   state = input;
   addRoundKey(rounds);

   for (uint8_t i = rounds - 1; i >= 1; --i)
   {
      inverseShiftRows();
      subBytes(inverse_sbox);
      addRoundKey(i);
      inverseMixColumns();
   }

   inverseShiftRows();
   subBytes(inverse_sbox);
   addRoundKey(0);
   
   return state;
}