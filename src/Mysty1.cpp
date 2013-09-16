#include "Mysty1.hpp"

constexpr uint8_t Mysty1::S7[];
constexpr uint16_t Mysty1::S9[];

void Mysty1::setKey(const BytesVector &key)
{
   if (key.size() != 16)
   {
      throw BadKeyLength("Your key length has to be 16 bytes.", key.size());
   }

   this->key = key;
}

uint16_t Mysty1::FI(const uint16_t in, const uint16_t K) const
{
   uint16_t d9 = in >> 7;
   uint8_t d7 = in & 0x7F;
   d9 = S9[d9] ^ d7;
   d7 = ((S7[d7] ^ d9) & 0x7F) ^ (K >> 9);
   d9 ^= K & 0x1FF;
   d9 = S9[d9] ^ d7;
   
   return (d7 << 9) | d9;
}

uint32_t Mysty1::FL(const uint32_t in, const uint8_t index) const
{
   uint16_t L = in >> 16;
   uint16_t R = in & 0xFFFF;
   
   if(!(index & 1))
   {
      R ^= (L & subkeys[index >> 1]);
      L ^= (R | subkeys[(((index >> 1) + 6) & 7) + 8]);
   }
   else
   {
      R ^= (L & subkeys[((((index - 1) >> 1) + 2) & 7) + 8]);
      L ^= (R | subkeys[(((index - 1) >> 1) + 4) & 7]);
   }
   
   return (L << 16) | R;
}

uint32_t Mysty1::FLInverse(const uint32_t in, const uint8_t index) const
{
   uint16_t L = in >> 16;
   uint16_t R = in & 0xFFFF;
   
   if(!(index & 1))
   {
      L ^= (R | subkeys[(((index >> 1) + 6) & 7) + 8]);
      R ^= (L & subkeys[index >> 1]);
   }
   else
   {
      L ^= (R | subkeys[(((index - 1) >> 1) + 4) & 7]);
      R ^= (L & subkeys[((((index - 1) >> 1) + 2) & 7) + 8]);
   }
   
   return (L << 16) | R;
}

void Mysty1::generateSubkeys()
{
   subkeys.resize(32);
   for(uint8_t i = 0; i < 8; ++i)
   {
      subkeys[i] = (key[i << 1] << 8) + key[2*i + 1];
   }
   
   for(uint8_t i = 0; i < 8; ++i)
   {
      subkeys[i + 8] = FI(subkeys[i], subkeys[(i + 1) & 7]);
      subkeys[i + 16] = subkeys[i + 8] & 0x1FF;
      subkeys[i + 24] = subkeys[i + 8] >> 9;
   }
}

const uint32_t Mysty1::F(const uint32_t half_block, const uint8_t index) const
{
   uint16_t L = half_block >> 16;
   uint16_t R = half_block & 0xFFFF;
   
   L ^= subkeys[index];
   L = FI(L, subkeys[((index + 5) & 7) + 8]) ^ R;
   
   R ^= subkeys[(index + 2) & 7];
   R = FI(R, subkeys[((index + 1) & 7) + 8]) ^ L;
   
   L ^= subkeys[(index + 7) & 7];
   L = FI(L, subkeys[((index + 3) & 7) + 8]) ^ R;
   
   R ^= subkeys[(index + 4) & 7];
   
   return (R << 16) | L;
}

void Mysty1::encodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const
{
   for(uint8_t i = 0; i < rounds; i += 2)
   {
      L = FL(L, i);
      R = FL(R, i + 1);
      R ^= F(L, i);
      L ^= F(R, i + 1);
   }
   L = FL(L, rounds);
   R = FL(R, rounds + 1);
}

void Mysty1::decodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const
{
   L = FLInverse(L, rounds);
   R = FLInverse(R, rounds + 1);
   for(int8_t i = rounds - 1; i >= 0; i -= 2)
   {
      L ^= F(R, i);
      R ^= F(L, i - 1);
      R = FLInverse(R, i);
      L = FLInverse(L, i - 1);
   }
}

const uint64_t Mysty1::encodeBlock(const uint64_t &input)
{
   uint32_t L = input >> 32;
   uint32_t R = input & 0xFFFFFFFF;
   encodeFeistelRounds(L, R, 0);
   
   return (static_cast<uint64_t>(R) << 32) | L;
}

const uint64_t Mysty1::decodeBlock(const uint64_t &input)
{
   uint32_t L = input >> 32;
   uint32_t R = input & 0xFFFFFFFF;
   decodeFeistelRounds(R, L, 0);
   
   return (static_cast<uint64_t>(R) << 32) | L;
}