#include "HC256.hpp"

#include "exceptions/BadKeyLength.hpp"

#include "BigEndian.hpp"
#include "LittleEndian.hpp"

void HC256::setKey(const BytesVector &key)
{
   const uint8_t key_size = key.size();
   if (key_size != 32)
   {
      throw BadKeyLength("Your key has to be 256 bits length.", key_size);
   }
   
   this->key = key;
   keySetup();
}

void HC256::setIV(const BytesVector &IV)
{
   const uint8_t iv_size = IV.size();
   if (iv_size != 32)
   {
      throw BadIVLength("Your IV has to be 256 bits length.", iv_size);
   }
   
   this->IV = IV;
}

uint32_t HC256::g(const uint32_t x, const uint32_t y, const UInt32Vector &K)
{
   return (Bits::rotateRight(x, 10) ^ Bits::rotateRight(y, 23)) + K[(x ^ y) & 0x3FF];
}

uint32_t HC256::h(const uint32_t x, const UInt32Vector &K)
{  
   return K[x & 0xFF] + K[256 + ((x >> 8) & 0xFF)] + K[512 + ((x >> 16) & 0xFF)] + K[768 + (x >> 24)];
}

void HC256::keySetup()
{
   P.reserve(1024);
   Q.reserve(1024);
   
   UInt32Vector W;
   W.reserve(2560);
   
   for(uint8_t i = 0; i < 32; i += 4)
   {
      W.push_back(BigEndian32::toInteger(BytesVector(key.begin() + i, key.begin() + i + 4)));
   }
   
   for(uint8_t i = 0; i < 32; i += 4)
   {
      W.push_back(BigEndian32::toInteger(BytesVector(IV.begin() + i, IV.begin() + i + 4)));
   }
   
   for(uint16_t i = 16; i < 2560; ++i)
   {
      W.push_back(F2(W[i - 2]) + W[i - 7] + F1(W[i - 15]) + W[i - 16] + i);
   }
   
   for(uint16_t i = 512; i < 1536; ++i)
   {
      P.push_back(W[i]);
      Q.push_back(W[i + 1024]);
   }
   
   for(uint8_t i = 0; i < 2; ++i)
   {
      for(uint16_t j = 0; j < 1024; ++j)
      {
         P[j] += calculateKey(P, Q, j);
      }     
      
      for(uint16_t j = 0; j < 1024; ++j)
      {
         Q[j] += calculateKey(Q, P, j);
      }
   }
}

uint32_t HC256::calculateKey(const UInt32Vector &PQ, const UInt32Vector &QP, const uint16_t i)
{
   return PQ[(i - 10) & 0x3FF] + g(PQ[(i - 3) & 0x3FF], PQ[(i - 1023) & 0x3FF], QP);
}

uint32_t HC256::updateSubkeys(UInt32Vector &K, const UInt32Vector &S, const uint16_t index)
{
   K[index] += calculateKey(K, S, index);
   
   return h(K[(index - 12) & 0x3FF], S) ^ K[index];
}

UInt32Vector HC256::generateKeystream()
{
   UInt32Vector keystream;
   keystream.reserve(2048);
   
   for(uint8_t j = 0; j < 2; ++j)
   {
      for(uint16_t i = 0; i < 1024; ++i)
      {
         keystream.push_back(updateSubkeys(P, Q, i));
      }
      
      for(uint16_t i = 0; i < 1024; ++i)
      {
         keystream.push_back(updateSubkeys(Q, P, i));
      }
   }
   
   return keystream;
}