#include "Scream.hpp"

#include "MathematicalTools.hpp"
#include "Vector.hpp"
#include "Endian.hpp"
#include "FunctionComposition.hpp"

#include "exceptions/BadKeyLength.hpp"
#include "Bits.hpp"

#include <algorithm>

constexpr std::array<uint8_t, 16> Scream::pi;
constexpr std::array<uint8_t, 256> Scream::sbox;

void Scream::setKey(const BytesVector &key)
{
   const uint8_t key_size = key.size();
   if (key_size != 16)
   {
      throw BadKeyLength("Your key has to be 128 bits length.", key_size);
   }
   this->key = key;
}

void Scream::setIV(const BytesVector &IV)
{
   const uint8_t iv_size = IV.size();
   if (iv_size != 16)
   {
      throw BadIVLength("Your IV has to be 128 bits length.", iv_size);
   }
   this->IV = IV;
}

void Scream_S::makeS1()
{
   S1.reserve(256);
   for(uint16_t x = 0; x < 256; ++x)
   {
      S1.push_back(composeSBox(16, 16, x));
   }
}

void Scream_0::makeS1()
{
   S1 = BytesVector(sbox.begin(), sbox.end());
}

void Scream::makeS2()
{
   S2.reserve(256);
   for(uint16_t x = 0; x < 256; ++x)
   {
      S2.push_back(S1[x ^ 0x15]);
   }
}

// Value 0x1C3 comes from the polynom x^8 + x^7 + x^6 + x + 1.
void Scream::makeT0()
{
   T0.reserve(256);
   for(uint16_t x = 0; x < 256; ++x)
   {
      T0.push_back((S1[x] << 24) | (Maths::GFMultiply(2, S1[x], 0x1C3) << 16)
           | (S2[x] << 8)  | Maths::GFMultiply(3, S2[x], 0x1C3));
   }
}

void Scream::makeT1()
{
   T1.reserve(256);
   for(uint16_t x = 0; x < 256; ++x)
   {
      T1.push_back((Maths::GFMultiply(2, S1[x], 0x1C3) << 24) | (S1[x] << 16)
           | (Maths::GFMultiply(3, S2[x], 0x1C3) << 8) | S2[x]);
   }
}

uint8_t Scream::composeSBox(const uint8_t n, const uint8_t max, const uint8_t x) const
{
   if(n > 1)
   {
      return composeSBox(n-1, max, sbox[(x + key[max - n]) & 0xFF]);
   }
   
   return sbox[(x + key[max - 1]) & 0xFF];
}

BytesVector Scream::F(const BytesVector &X)
{
   std::array<uint32_t, 4> u = {{T0[X[0]] ^ T1[X[13]], T0[X[4]] ^ T1[X[1]],
                    T0[X[8]] ^ T1[X[5]], T0[X[12]] ^ T1[X[9]]}};
   
   std::array<uint16_t, 4> bytes23 = {{static_cast<uint16_t>(u[0]), static_cast<uint16_t>(u[1]),
                      static_cast<uint16_t>(u[2]), static_cast<uint16_t>(u[3])}};
   for(uint8_t i = 0; i < 4; ++i)
   {
      const uint8_t j = (i * 4) + 2;
      bytes23[i] ^= (X[j] << 8) | X[1 + j];
      u[i] >>= 16;
      u[i] |= (bytes23[i] << 16);
   }
   
   // Second half-round.
   std::array<uint32_t, 4> x = {{T0[u[2] >> 24] ^ T1[(u[1] >> 16) & 0xFF], 
                    T0[u[3] >> 24] ^ T1[(u[2] >> 16) & 0xFF],
                    T0[u[0] >> 24] ^ T1[(u[3] >> 16) & 0xFF], 
                    T0[u[1] >> 24] ^ T1[(u[0] >> 16) & 0xFF]}};
   
   BytesVector out;
   out.reserve(16);
   for(uint8_t i = 0; i < 4; ++i)
   {
      bytes23[i] = (x[i] & 0xFFFF) ^ (u[i] & 0xFFFF);
      x[i] &= 0xFFFF0000;
      x[i] |= bytes23[i];
      
      out.extend(BigEndian32::toBytesVector(x[i]));
   }
   
   return out;
}

void Scream::IVSetup()
{
   const auto F = std::bind(&Scream::F, this, std::placeholders::_1);
   Z = compose<2>(F)(IV.Xor(subkeys[1]));
   Y = compose<2>(F)(Z.Xor(subkeys[3]));
   const BytesVector A = compose<2>(F)(Y.Xor(subkeys[5]));
   X = F(A.Xor(subkeys[7]));
   BytesVector B(X);
   
   for(uint8_t i = 0; i < 8; ++i)
   {
      const uint8_t j = i * 2;
      B = F(B.Xor(subkeys[j]));
      subkeys[j] = subkeys[j].Xor(A);
      subkeys[j+1] = subkeys[j+1].Xor(B);
   }
}

void Scream::keySetup()
{
   makeS1();
   makeS2();
   
   makeT0();
   makeT1();
   
   const BytesVector B = F(key.Xor(BytesVector(pi.begin(), pi.end())));
   subkeys.reserve(16);
   BytesVector A(key);
   for(uint8_t i = 0; i < 16; ++i)
   {
      A = compose<4>(std::bind(&Scream::F, this, std::placeholders::_1))(A).Xor(B);
      subkeys.push_back(A);
   }
}

BytesVector Scream::generateKeystream()
{  
   keySetup();
   IVSetup();
   
   BytesVector keystream;
   keystream.reserve(256);
   for(uint8_t i = 0; i < 16; ++i)
   {
      X = F(X.Xor(Y)).Xor(Z);
      keystream.extend(X.Xor(subkeys[i % 16]));

      switch(i % 4)
      {
         case 0:
         case 2:
            std::rotate(Y.begin(), Y.begin() + 8, Y.end());
            break;

         case 1:
            std::rotate(Y.begin(), Y.begin() + 4, Y.begin() + 8);
            std::rotate(Y.begin() + 8, Y.begin() + 12, Y.end());
            break;

         default:
            if(i < 15)
            {
               std::rotate(Y.begin(), Y.begin() + 5, Y.begin() + 8);
               std::rotate(Y.begin() + 8, Y.begin() + 13, Y.end());
            }
      }      
   }
   Y = F(Y.Xor(Z));
   Z = F(Z.Xor(Y));
   subkeys[counter] = F(subkeys[counter]);
   counter = (counter + 1) % 16;
   
   return keystream;
}

BytesVector Scream::encode(const BytesVector &message)
{   
   const uint64_t output_size = message.size();
   BytesVector output;
   output.reserve(output_size);
   
   for(uint64_t j = 0; j < output_size; j += 256)
   {
      const BytesVector keystream = generateKeystream();
      for(uint16_t i = 0; i < 256; ++i)
      {
         const uint64_t sum_indices = i + j;
         if(output_size == sum_indices)
         {
            return output;
         }
         output.push_back(message[sum_indices] ^ keystream[i]);
      }
   }
   
   return output;
}