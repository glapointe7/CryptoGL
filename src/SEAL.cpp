
#include "Types.hpp"

#include "SEAL.hpp"

#include "SHA1.hpp"
#include "Bits.hpp"
#include "BigEndian.hpp"
#include "Tools.hpp"

#include "exceptions/BadKeyLength.hpp"

void SEAL::setKey(const BytesVector &key)
{
   if (key.size() != 20)
   {
      throw BadKeyLength("Your key has to have 20 bytes length.", key.size());
   }

   this->key = key;
   keySetup();
}

uint32_t SEAL::gamma(const uint32_t index, uint32_t &previous_index)
{   
   const uint32_t current_index = index / 5;
   if(current_index != previous_index)
   {
      // Get the block = (i/5) || 0^{60}.
      UInt32Vector block(16, 0);
      block[0] = current_index;
      
      // Apply G_a(i) to the block.
      SHA1 *G = new SHA1();
      H = IV;
      G->compress(block, H);
      delete G;
      previous_index = current_index;
   }
   
   return H[index % 5];
}

void SEAL::keySetup()
{
   // The key is the new IV for SHA-1.
   IV.reserve(5);
   for(uint8_t i = 0; i < 20; i += 4)
   {
      IV.push_back(BigEndian32::toInteger(BytesVector(key.begin() + i, key.begin() + i + 4)));
   }
   uint32_t previous_index = 0xFFFFFFFF;
   
   T.reserve(512);
   for(uint16_t i = 0; i < 512; ++i)
   {
      T.push_back(gamma(i, previous_index));
   }
   
   S.reserve(256);
   for(uint16_t i = 0; i < 256; ++i)
   {
      S.push_back(gamma(i + 0x1000, previous_index));
   }
   
   R.reserve(4 * (output_size / 1024));
   const uint16_t upper_bound = 4 * (output_size / 1024);
   for(uint16_t i = 0; i < upper_bound; ++i)
   {
      R.push_back(gamma(i + 0x2000, previous_index));
   }
}

void SEAL::initialize(const uint8_t index, UInt32Vector &A, UInt32Vector &registers) const
{
   for(uint8_t i = 0; i < 4; ++i)
   {
      A[i] = Bits::rotateRight(seed, (i << 3)) ^ R[4*index + i];
   }
   
   uint16_t P;
   for(uint8_t i = 0; i < 2; ++i)
   {
      for(uint8_t j = 0; j < 4; ++j)
      {
         P = A[j] & 0x7FC;
         A[(j+1) & 3] += T[P / 4];
         A[j] = Bits::rotateRight(A[j], 9);
      }
   }
   
   registers[0] = A[3];
   registers[1] = A[1];
   registers[2] = A[0];
   registers[3] = A[2];
   
   for(uint8_t j = 0; j < 4; ++j)
   {
      P = A[j] & 0x7FC;
      A[(j+1) & 3] += T[P / 4];
      A[j] = Bits::rotateRight(A[j], 9);
   }
}

UInt32Vector SEAL::generateKeystream()
{
   UInt32Vector keystream;
   keystream.reserve(output_size);
      
   UInt32Vector A(4, 0);
   UInt32Vector registers(A);
   uint16_t P, Q;
   const uint8_t number_of_Kb = output_size / 1024;
   for(uint8_t l = 0; l < number_of_Kb; ++l)
   {
      initialize(l, A, registers);
      for(uint8_t i = 0; i < 64; ++i)
      {
         P = A[0] & 0x7FC;
         A[1] += T[P / 4];
         A[0] = Bits::rotateRight(A[0], 9);
         A[1] ^= A[0];
         
         Q = A[1] & 0x7FC;
         A[2] ^= T[Q / 4];
         A[1] = Bits::rotateRight(A[1], 9);
         A[2] += A[1];
         
         P = (P + A[2]) & 0x7FC;
         A[3] += T[P / 4];
         A[2] = Bits::rotateRight(A[2], 9);
         A[3] ^= A[2];
         
         Q = (Q + A[3]) & 0x7FC;
         A[0] ^= T[Q / 4];
         A[3] = Bits::rotateRight(A[3], 9);
         A[0] += A[3];
         
         
         P = (P + A[0]) & 0x7FC;
         A[1] ^= T[P / 4];
         A[0] = Bits::rotateRight(A[0], 9);
         
         Q = (Q + A[1]) & 0x7FC;
         A[2] += T[Q / 4];
         A[1] = Bits::rotateRight(A[1], 9);
         
         P = (P + A[2]) & 0x7FC;
         A[3] ^= T[P / 4];
         A[2] = Bits::rotateRight(A[2], 9);
         
         Q = (Q + A[3]) & 0x7FC;
         A[0] += T[Q / 4];
         A[3] = Bits::rotateRight(A[3], 9);
         
         const uint8_t j = 4 * i;         
         keystream.push_back(A[1] + S[j]);
         keystream.push_back(A[2] ^ S[j + 1]);
         keystream.push_back(A[3] + S[j + 2]);
         keystream.push_back(A[0] ^ S[j + 3]);
         
         if(i & 1)
         {
            A[0] += registers[2];
            A[1] += registers[3];
            A[2] ^= registers[2];
            A[3] ^= registers[3];
         }
         else
         {
            A[0] += registers[0];
            A[1] += registers[1];
            A[2] ^= registers[0];
            A[3] ^= registers[1];
         }
      }
   }

   return keystream;
}

const BytesVector SEAL::encode(const BytesVector &message)
{   
   seed = 0;
   output_size = message.size();
   
   BytesVector output;
   output.reserve(output_size);
   
   for(uint16_t j = 0; j < output_size; j += 1024)
   {
      for(uint16_t i = 0; i < 1024; ++i)
      {
         UInt32Vector keystream = generateKeystream();
         output.push_back(message[i+j] ^ keystream[i]);
      }
      seed++;
   }
   
   return output;
}