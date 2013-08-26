#include "Twofish.hpp"

#include <algorithm>

#include "LittleEndian.hpp"
#include "BigEndian.hpp"
#include "Bits.hpp"

#include "exceptions/BadKeyLength.hpp"

constexpr uint8_t Twofish::RS[][8];
constexpr uint8_t Twofish::MDS[][4];
//constexpr uint8_t Twofish::t[][4][16];
constexpr uint8_t Twofish::Q[][256];

void Twofish::setKey(const BytesContainer &key)
{
   if (key.size() != 16 && key.size() != 24 && key.size() != 32)
   {
      throw BadKeyLength("Your key length has to be of length 16, 24 or 32 bytes.", key.size());
   }

   this->key = key;
}

uint8_t Twofish::GFMultiply(const uint8_t a, uint8_t b, const uint16_t p)
{
   uint16_t A = a;
   uint8_t prod = 0;
   while(b > 0)
   {
      if(b & 1)
      {
         prod ^= A;
      }
      A <<= 1;
      b >>= 1;
      if(A & 0x100)
      {
         A ^= p;
      }
   }
   
   return prod;
}

uint32_t Twofish::h(const uint32_t X, const BytesContainer &L) const
{
   // Split X into 4 bytes in little endian.
   LittleEndian32 LE;
   LE.toBytes(X);
   
   const uint8_t k = L.size() >> 2;
   BytesContainer y(LE.getBytes());
   if(k == 4)
   {
      constexpr uint8_t k4[4] = {1, 0, 0, 1};
      for(uint8_t i = 0; i < 4; ++i)
      {
         y[i] = Q[k4[i]][y[i]] ^ L[12 + i];
      }
   }
         
   if(k >= 3)
   {
      constexpr uint8_t k3[4] = {1, 1, 0, 0};
      for(uint8_t i = 0; i < 4; ++i)
      {
         y[i] = Q[k3[i]][y[i]] ^ L[8 + i];
      }
   }
         
   constexpr uint8_t k2[12] = {1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1};
   for(uint8_t i = 0; i < 4; ++i)
   {
      y[i] = Q[k2[i]][Q[k2[4 + i]][Q[k2[8 + i]][y[i]] ^ L[4 + i]] ^ L[i]];
   }
   
   // v(x) = x^8 + x^6 + x^5 + x^3 + 1 over GF(2). (101101001)_2 = 0x169.
   BytesContainer z(4, 0);
   for(uint8_t i = 0; i < 4; ++i)
   {
      for(uint8_t j = 0; j < 4; ++j)
      {
         z[i] ^= GFMultiply(MDS[i][j], y[j], 0x169);
      }
   }
   LE.toInteger(z);
   
   return LE.getValue();
}

void Twofish::generateSubkeys()
{
   subkeys.reserve(40);
   const uint8_t k = key.size() >> 3;
   const uint8_t k4 = k << 2;
   BytesContainer Me, Mo;
   Me.reserve(k4);
   Mo.reserve(k4);
   std::vector<BytesContainer> S(k, BytesContainer(4, 0));

   for (uint8_t i = 0; i < k; ++i)
   {  
      const uint8_t x = i << 3;
      Me.insert(Me.end(), key.begin() + x, key.begin() + x + 4);
      Mo.insert(Mo.end(), key.begin() + x + 4, key.begin() + x + 8);
      
      // We multiply RS by the vector key_{8i + j} where j = 0,...,7.
      // w(x) = x^8 + x^6 + x^3 + x^2 + 1 over GF(2). (101001101)_2 = 0x14D.
      for(uint8_t j = 0; j < 4; ++j)
      {
         for(uint8_t l = 0; l < 8; ++l)
         {
            S[i][j] ^= GFMultiply(RS[j][l], key[x + l], 0x14D);
         }
      }
   }
   // s = (s_{k-1}, ..., s_0) => s in little endian and reverse order.
   s.reserve(k4);
   for(uint8_t i = 0; i < k; ++i)
   {
      s.insert(s.end(), S[i].rbegin(), S[i].rend());
   }
   std::reverse(s.begin(), s.end());
   
   // Expand the key to get the 40 subkeys.
   constexpr uint32_t rho = 0x01010101;
   constexpr uint32_t rho2 = 0x02020202;
   for(uint8_t i = 0; i < 20; ++i)
   {
      const uint32_t A = h(i * rho2, Me);
      const uint32_t B = Bits::rotateLeft(h(rho2*i + rho, Mo), 8, 32);
      subkeys.push_back(A + B);
      subkeys.push_back(Bits::rotateLeft(A + 2*B, 9, 32));
   }
}

uint32_t Twofish::g(const uint32_t X) const
{
   return h(X, s);
}

const Twofish::UInt32Container Twofish::F(const UInt32Container half_block, const uint8_t round) const
{
   const uint32_t T0 = g(half_block[0]);
   const uint32_t T1 = g(Bits::rotateLeft(half_block[1], 8, 32));
   
   return {T0 + T1 + subkeys[2*round + 8], T0 + 2*T1 + subkeys[2*round + 9]};
}

void Twofish::encodeFeistelRounds(UInt32Container &L, UInt32Container &R, const uint8_t) const
{
   for(uint8_t i = 0; i < rounds; ++i)
   {
      const UInt32Container F_result = F(R, i);
      L[0] = Bits::rotateRight(L[0] ^ F_result[0], 1, 32);
      L[1] = Bits::rotateLeft(L[1], 1, 32) ^ F_result[1];
      std::swap(R[0], L[0]);
      std::swap(R[1], L[1]);
   }
   
   // Undo the 2 last swaps.
   std::swap(R[0], L[0]);
   std::swap(R[1], L[1]);
}

void Twofish::decodeFeistelRounds(UInt32Container &L, UInt32Container &R, const uint8_t) const
{
   std::swap(R[0], L[0]);
   std::swap(R[1], L[1]);
   
   for(int8_t i = rounds - 1; i >= 0; --i)
   {
      std::swap(R[0], L[0]);
      std::swap(R[1], L[1]);
      const UInt32Container F_result = F(R, i);
      L[0] = Bits::rotateLeft(L[0], 1, 32) ^ F_result[0];
      L[1] = Bits::rotateRight(L[1] ^ F_result[1], 1, 32);
   }
}

const Twofish::UInt32Container Twofish::encodeBlock(const UInt32Container &input)
{
   // Input whitening.
   UInt32Container encoded_block(input);
   for(uint8_t i = 0; i < 4; ++i)
   {
      encoded_block[i] ^= subkeys[i];
   }
   
   UInt32Container R = {encoded_block[0], encoded_block[1]};
   UInt32Container L = {encoded_block[2], encoded_block[3]};
   encodeFeistelRounds(L, R, 0);
   encoded_block[0] = R[0];
   encoded_block[1] = R[1];
   encoded_block[2] = L[0];
   encoded_block[3] = L[1];
   
   // Output whitening.
   for(uint8_t i = 0; i < 4; ++i)
   {
      encoded_block[i] ^= subkeys[i + 4];
   }
   
   return encoded_block;
}

const Twofish::UInt32Container Twofish::decodeBlock(const UInt32Container &input)
{
   // Input whitening.
   UInt32Container decoded_block(input);
   for(uint8_t i = 0; i < 4; ++i)
   {
      decoded_block[i] ^= subkeys[i + 4];
   }
   
   UInt32Container R = {decoded_block[0], decoded_block[1]};
   UInt32Container L = {decoded_block[2], decoded_block[3]};
   decodeFeistelRounds(L, R, 0);
   decoded_block[0] = R[0];
   decoded_block[1] = R[1];
   decoded_block[2] = L[0];
   decoded_block[3] = L[1];
   
   for(uint8_t i = 0; i < 4; ++i)
   {
      decoded_block[i] ^= subkeys[i];
   }
   
   return decoded_block;
}