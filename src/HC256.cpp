#include "HC256.hpp"

#include "exceptions/BadKeyLength.hpp"

#include "BigEndian.hpp"
#include "LittleEndian.hpp"

void HC256::setKey(const BytesVector &key)
{
   if (key.size() != 32)
   {
      throw BadKeyLength("Your key has to be 256 bits length.", key.size());
   }
   
   this->key = key;
}

void HC256::setIV(const BytesVector &IV)
{
   if (IV.size() != 32)
   {
      throw BadIVLength("Your IV has to be 256 bits length.", IV.size());
   }
   
   this->IV = IV;
   generateSubkeys();
}

uint32_t HC256::g(const uint32_t x, const uint32_t y, const UInt32Vector &K)
{
   return (Bits::rotateRight(x, 10) ^ Bits::rotateRight(y, 23)) + K[(x ^ y) & 0x3FF];
}

uint32_t HC256::h(const uint32_t x, const UInt32Vector &K)
{
   const BytesVector X = LittleEndian32::toBytesVector(x);
   
   return K[X[0]] + K[256 + X[1]] + K[512 + X[2]] + K[768 + X[3]];
}

void HC256::generateSubkeys()
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
   
   for(uint16_t i = 0; i < 1024; ++i)
   {
      P.push_back(W[i + 512]);
      Q.push_back(W[i + 1536]);
   }
   
   for(uint8_t i = 0; i < 2; ++i)
   {
      for(uint16_t j = 0; j < 1024; ++j)
      {
         P[j] += P[(j - 10) & 0x3FF] + g(P[(j - 3) & 0x3FF], P[(j - 1023) & 0x3FF], Q);
      }
      
      for(uint16_t j = 0; j < 1024; ++j)
      {
         Q[j] += Q[(j - 10) & 0x3FF] + g(Q[(j - 3) & 0x3FF], Q[(j - 1023) & 0x3FF], P);
      }
   }
}

uint32_t HC256::updateSubkeys(UInt32Vector &K, const UInt32Vector &S, const uint16_t index)
{
   K[index] += K[(index - 10) & 0x3FF] + g(K[(index - 3) & 0x3FF], K[(index - 1023) & 0x3FF], S);
   
   return h(K[(index - 12) & 0x3FF], S) ^ K[index];
}

uint32_t HC256::generateKeystream(const uint64_t &index)
{
   uint32_t keystream;
   uint16_t j = index & 0x3FF;
   if((index & 0x7FF) < 1024)
   {
      keystream = updateSubkeys(P, Q, j);
   }
   else
   {
      keystream = updateSubkeys(Q, P, j);
   }
   
   return keystream;
}

const BytesVector HC256::encode(const BytesVector &message)
{
   const uint64_t output_size = message.size();
   BytesVector output;
   output.reserve(output_size);
   
   for(uint64_t i = 0; i < output_size; i += 4)
   {
      const uint32_t keystream = generateKeystream(i >> 2);
      const BytesVector key_bytes = BigEndian32::toBytesVector(keystream);
      for(uint8_t j = 0; j < 4; ++j)
      {
         output.push_back(message[i + j] ^ key_bytes[j]);
      }
   }
   
   return output;
}

const BytesVector HC256::decode(const BytesVector &message)
{
   return encode(message);
}