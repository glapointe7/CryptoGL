#include "Twofish.hpp"

#include <algorithm>

#include "Endian.hpp"
#include "Endian.hpp"
#include "Bits.hpp"
#include "MathematicalTools.hpp"

#include "exceptions/BadKeyLength.hpp"

constexpr std::array<std::array<uint8_t, 8>, 4> Twofish::RS;
constexpr std::array<std::array<uint8_t, 4>, 4> Twofish::MDS;
constexpr std::array<std::array<uint8_t, 256>, 4> Twofish::Q;

void Twofish::setKey(const BytesVector &key)
{
   const uint8_t key_size = key.size();
   if (key_size != 16 && key_size != 24 && key_size != 32)
   {
      throw BadKeyLength("Your key length has to be of length 16, 24 or 32 bytes.", key_size);
   }

   this->key = key;
}

uint32_t Twofish::h(const uint32_t X, const BytesVector &L)
{
   // Split X into 4 bytes in little endian.
   const uint8_t k = L.size() / 4;
   BytesVector y = LittleEndian32::toBytesVector(X);
   switch(k)
   {
      case 4 :
      {
         constexpr std::array<uint8_t, 4> k4 = {{1, 0, 0, 1}};
         for(uint8_t i = 0; i < 4; ++i)
         {
            y[i] = Q[k4[i]][y[i]] ^ L[12 + i];
         }
      }
         
      case 3:
      {
         constexpr std::array<uint8_t, 4> k3 = {{1, 1, 0, 0}};
         for(uint8_t i = 0; i < 4; ++i)
         {
            y[i] = Q[k3[i]][y[i]] ^ L[8 + i];
         }
      }
   }
         
   constexpr std::array<uint8_t, 12> k2 = {{1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1}};
   for(uint8_t i = 0; i < 4; ++i)
   {
      y[i] = Q[k2[i]][Q[k2[4 + i]][Q[k2[8 + i]][y[i]] ^ L[4 + i]] ^ L[i]];
   }
   
   // v(x) = x^8 + x^6 + x^5 + x^3 + 1 over GF(2). (101101001)_2 = 0x169.
   BytesVector z(4, 0);
   for(uint8_t i = 0; i < 4; ++i)
   {
      for(uint8_t j = 0; j < 4; ++j)
      {
         z[i] ^= Maths::GFMultiply(MDS[i][j], y[j], 0x169);
      }
   }
   
   return LittleEndian32::toInteger(z);
}

void Twofish::generateSubkeys()
{
   subkeys.reserve(40);
   const uint8_t k = key.size() / 8;
   const uint8_t k4 = k * 4;
   BytesVector Me, Mo;
   Me.reserve(k4);
   Mo.reserve(k4);
   BytesMatrix S(k, BytesVector(4, 0));

   for (uint8_t i = 0; i < k; ++i)
   {  
      const uint8_t x = i * 8;
      Vector::extend(Me, key, x, x+4);
      Vector::extend(Mo, key, x+4, x+8);
      
      // We multiply RS by the vector key_{8i + j} where j = 0,...,7.
      // w(x) = x^8 + x^6 + x^3 + x^2 + 1 over GF(2). (101001101)_2 = 0x14D.
      for(uint8_t j = 0; j < 4; ++j)
      {
         for(uint8_t l = 0; l < 8; ++l)
         {
            S[i][j] ^= Maths::GFMultiply(RS[j][l], key[x + l], 0x14D);
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
      const uint32_t x = h(i * rho2, Me);
      const uint32_t y = Bits::rotateLeft(h(rho2*i + rho, Mo), 8);
      subkeys.push_back(x + y);
      subkeys.push_back(Bits::rotateLeft(x + 2*y, 9));
   }
}

UInt32Vector Twofish::F(const UInt32Vector half_block, const uint8_t round) const
{
   const uint32_t T0 = h(half_block[0], s);
   const uint32_t T1 = h(Bits::rotateLeft(half_block[1], 8), s);
   
   return {T0 + T1 + subkeys[2*round + 8], T0 + 2*T1 + subkeys[2*round + 9]};
}

void Twofish::encodeFeistelRounds(UInt32Vector &L, UInt32Vector &R, const uint8_t) const
{
   for(uint8_t i = 0; i < rounds; ++i)
   {
      const UInt32Vector F_result = F(R, i);
      L[0] = Bits::rotateRight(L[0] ^ F_result[0], 1);
      L[1] = Bits::rotateLeft(L[1], 1) ^ F_result[1];
      std::swap(R[0], L[0]);
      std::swap(R[1], L[1]);
   }
}

void Twofish::decodeFeistelRounds(UInt32Vector &L, UInt32Vector &R, const uint8_t) const
{
   for(int8_t i = rounds - 1; i >= 0; --i)
   {
      std::swap(R[0], L[0]);
      std::swap(R[1], L[1]);
      const UInt32Vector F_result = F(R, i);
      L[0] = Bits::rotateLeft(L[0], 1) ^ F_result[0];
      L[1] = Bits::rotateRight(L[1] ^ F_result[1], 1);
   }
}

UInt32Vector Twofish::encodeBlock(const UInt32Vector &input)
{
   // Input whitening.
   UInt32Vector encoded_block(input);
   for(uint8_t i = 0; i < 4; ++i)
   {
      encoded_block[i] ^= subkeys[i];
   }
   
   UInt32Vector R = {encoded_block[0], encoded_block[1]};
   UInt32Vector L = {encoded_block[2], encoded_block[3]};
   encodeFeistelRounds(L, R, 0);
   encoded_block[0] = L[0];
   encoded_block[1] = L[1];
   encoded_block[2] = R[0];
   encoded_block[3] = R[1];
   
   // Output whitening.
   for(uint8_t i = 0; i < 4; ++i)
   {
      encoded_block[i] ^= subkeys[i + 4];
   }
   
   return encoded_block;
}

UInt32Vector Twofish::decodeBlock(const UInt32Vector &input)
{
   // Input whitening.
   UInt32Vector decoded_block(input);
   for(uint8_t i = 0; i < 4; ++i)
   {
      decoded_block[i] ^= subkeys[i + 4];
   }
   
   UInt32Vector L = {decoded_block[0], decoded_block[1]};
   UInt32Vector R = {decoded_block[2], decoded_block[3]};
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