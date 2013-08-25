#include "CAST128.hpp"

#include "BigEndian.hpp"
#include "Bits.hpp"

constexpr uint32_t CAST128::S[][256];

void CAST128::setKey(const BytesContainer &key)
{
   if (key.size() < 5 || key.size() > 16)
   {
      throw BadKeyLength("Your key length has to be between 5 bytes and 16 bytes.", key.size());
   }
   
   if(key.size() > 10)
   {
      rounds = 16;
   }
   
   // Pad the key with 0 to get 128 bits length.
   BytesContainer key_padded(key);
   if(key.size() < 16)
   {
      key_padded.reserve(16);
      key_padded.insert(key_padded.end(), 16 - key.size(), 0);
   }

   this->key = key_padded;
}

void CAST128::setTempKeyZ(UInt32Container &Z, const UInt32Container &X) const
{
   Z[0] = X[0] ^ S[4][getByteFromInteger(X, 0xD)] ^ S[5][getByteFromInteger(X, 0xF)]
           ^ S[6][getByteFromInteger(X, 0xC)] ^ S[7][getByteFromInteger(X, 0xE)]
           ^ S[6][getByteFromInteger(X, 8)];
   
   Z[1] = X[2] ^ S[4][getByteFromInteger(Z, 0)] ^ S[5][getByteFromInteger(Z, 2)]
           ^ S[6][getByteFromInteger(Z, 1)] ^ S[7][getByteFromInteger(Z, 3)]
           ^ S[7][getByteFromInteger(X, 0xA)];
   
   Z[2] = X[3] ^ S[4][getByteFromInteger(Z, 7)] ^ S[5][getByteFromInteger(Z, 6)]
           ^ S[6][getByteFromInteger(Z, 5)] ^ S[7][getByteFromInteger(Z, 4)]
           ^ S[4][getByteFromInteger(X, 9)];
   
   Z[3] = X[1] ^ S[4][getByteFromInteger(Z, 0xA)] ^ S[5][getByteFromInteger(Z, 9)]
           ^ S[6][getByteFromInteger(Z, 0xB)] ^ S[7][getByteFromInteger(Z, 8)]
           ^ S[5][getByteFromInteger(X, 0xB)];
}

void CAST128::setTempKeyX(UInt32Container &X, const UInt32Container &Z) const
{
   X[0] = Z[2] ^ S[4][getByteFromInteger(Z, 5)] ^ S[5][getByteFromInteger(Z, 7)]
           ^ S[6][getByteFromInteger(Z, 4)] ^ S[7][getByteFromInteger(Z, 6)]
           ^ S[6][getByteFromInteger(Z, 0)];
   
   X[1] = Z[0] ^ S[4][getByteFromInteger(X, 0)] ^ S[5][getByteFromInteger(X, 2)]
           ^ S[6][getByteFromInteger(X, 1)] ^ S[7][getByteFromInteger(X, 3)]
           ^ S[7][getByteFromInteger(Z, 2)];
   
   X[2] = Z[1] ^ S[4][getByteFromInteger(X, 7)] ^ S[5][getByteFromInteger(X, 6)]
           ^ S[6][getByteFromInteger(X, 5)] ^ S[7][getByteFromInteger(X, 4)]
           ^ S[4][getByteFromInteger(Z, 1)];
   
   X[3] = Z[3] ^ S[4][getByteFromInteger(X, 0xA)] ^ S[5][getByteFromInteger(X, 9)]
           ^ S[6][getByteFromInteger(X, 0xB)] ^ S[7][getByteFromInteger(X, 8)]
           ^ S[5][getByteFromInteger(Z, 3)];
}

void CAST128::setSubKeysBlock(const UInt32Container &tmp, const uint8_t *index)
{
   for(uint8_t i = 0; i < 4; ++i)
   {
      const uint8_t j = 5 * i;
      subkeys.push_back(S[4][getByteFromInteger(tmp, index[j])] ^ S[5][getByteFromInteger(tmp, index[j+1])] 
         ^ S[6][getByteFromInteger(tmp, index[j+2])] ^ S[7][getByteFromInteger(tmp, index[j+3])] 
         ^ S[4 + i][getByteFromInteger(tmp, index[j+4])]);
   }
}

void CAST128::generateSubkeys()
{
   subkeys.reserve(32);
   UInt32Container X(4, 0);
   
   BigEndian32 BE;
   for(uint8_t i = 0; i < 16; i += 4)
   {
      BE.toInteger(BytesContainer(key.begin() + i, key.begin() + i + 4));
      X[i >> 2] = BE.getValue();
      BE.resetValue();
   }
   
   constexpr uint8_t indexes[4][20] = {
      {8, 9, 7, 6, 2, 10, 11, 5, 4, 6, 12, 13, 3, 2, 9, 14, 15, 1, 0, 12},
      {3, 2, 12, 13, 8, 1, 0, 14, 15, 13, 7, 6, 8, 9, 3, 5, 4, 10, 11, 7},
      {3, 2, 12, 13, 9, 1, 0, 14, 15, 12, 7, 6, 8, 9, 2, 5, 4, 10, 11, 6},
      {8, 9, 7, 6, 3, 10, 11, 5, 4, 7, 12, 13, 3, 2, 8, 14, 15, 1, 0, 13}
   };
   
   UInt32Container Z(4, 0);
   for(uint8_t i = 0; i < 4; ++i)
   {
      setTempKeyZ(Z, X);
      setSubKeysBlock(Z, indexes[(2*i) & 3]);
      
      setTempKeyX(X, Z);
      setSubKeysBlock(X, indexes[(2*i + 1) & 3]);
   }
}

const uint32_t CAST128::F(const uint32_t half_block, const uint8_t index) const
{
   const uint32_t I = Bits::rotateLeft(subkeys[index] + half_block, subkeys[index + 16] & 0x1F, 32);
   return ((S[0][getByteFromInteger(I, 3)] ^ S[1][getByteFromInteger(I, 2)])
           - S[2][getByteFromInteger(I, 1)]) + S[3][getByteFromInteger(I, 0)];
}

uint32_t CAST128::F2(const uint32_t half_block, const uint8_t index) const
{
   const uint32_t I = Bits::rotateLeft(subkeys[index] ^ half_block, subkeys[index + 16] & 0x1F, 32);
   return ((S[0][getByteFromInteger(I, 3)] - S[1][getByteFromInteger(I, 2)])
           + S[2][getByteFromInteger(I, 1)]) ^ S[3][getByteFromInteger(I, 0)];
}

uint32_t CAST128::F3(const uint32_t half_block, const uint8_t index) const
{
   const uint32_t I = Bits::rotateLeft(subkeys[index] - half_block, subkeys[index + 16] & 0x1F, 32);
   return ((S[0][getByteFromInteger(I, 3)] + S[1][getByteFromInteger(I, 2)])
           ^ S[2][getByteFromInteger(I, 1)]) - S[3][getByteFromInteger(I, 0)];
}

void CAST128::encodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const
{
   L ^= F(R, 0);
   R ^= F2(L, 1);
   L ^= F3(R, 2);
   
   R ^= F(L, 3);
   L ^= F2(R, 4);
   R ^= F3(L, 5);
   
   L ^= F(R, 6);
   R ^= F2(L, 7);
   L ^= F3(R, 8);
   
   R ^= F(L, 9);
   L ^= F2(R, 10);
   R ^= F3(L, 11);
   
   if(rounds > 12)
   {
      L ^= F(R, 12);
      R ^= F2(L, 13);
      L ^= F3(R, 14);
      R ^= F(L, 15);
   }
}

void CAST128::decodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const
{
   if(rounds > 12)
   {
      L ^= F(R, 15);
      R ^= F3(L, 14);
      L ^= F2(R, 13);
      R ^= F(L, 12);
   }
   
   L ^= F3(R, 11);
   R ^= F2(L, 10);
   L ^= F(R, 9);
   
   R ^= F3(L, 8);
   L ^= F2(R, 7);
   R ^= F(L, 6);
   
   L ^= F3(R, 5);
   R ^= F2(L, 4);
   L ^= F(R, 3);
   
   R ^= F3(L, 2);
   L ^= F2(R, 1);
   R ^= F(L, 0);
}

const uint64_t CAST128::getIntegersFromInputBlock(const BytesContainer &block) const
{
   BigEndian64 BE;
   BE.toInteger(block);
   
   return BE.getValue();
}

const uint64_t CAST128::encodeBlock(const uint64_t &input)
{
   uint32_t L = input >> 32;
   uint32_t R = input & 0xFFFFFFFF;
   encodeFeistelRounds(L, R, 0);
   
   return (static_cast<uint64_t>(R) << 32) | L;
}
const uint64_t CAST128::decodeBlock(const uint64_t &input)
{
   uint32_t L = input >> 32;
   uint32_t R = input & 0xFFFFFFFF;
   decodeFeistelRounds(L, R, 0);
   
   return (static_cast<uint64_t>(R) << 32) | L;
}