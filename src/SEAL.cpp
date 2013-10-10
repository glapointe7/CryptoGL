
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
   
   const uint16_t upper_bound = 4 * (output_size / 1024);
   R.reserve(upper_bound);
   for(uint16_t i = 0; i < upper_bound; ++i)
   {
      R.push_back(gamma(i + 0x2000, previous_index));
   }
}

void SEAL::initialize()
{
   for(uint8_t i = 0; i < 4; ++i)
   {
      state[i] = Bits::rotateRight(seed, (i << 3)) ^ R[4*counter + i];
   }
   
   uint16_t P;
   for(uint8_t i = 0; i < 2; ++i)
   {
      for(uint8_t j = 0; j < 4; ++j)
      {
         P = state[j] & 0x7FC;
         state[(j+1) & 3] += T[P / 4];
         state[j] = Bits::rotateRight(state[j], 9);
      }
   }
   
   state[4] = state[3];
   state[5] = state[1];
   state[6] = state[0];
   state[7] = state[2];
   
   for(uint8_t j = 0; j < 4; ++j)
   {
      P = state[j] & 0x7FC;
      state[(j+1) & 3] += T[P / 4];
      state[j] = Bits::rotateRight(state[j], 9);
   }
}

UInt32Vector SEAL::generateKeystream()
{
   UInt32Vector keystream;
   keystream.reserve(1024);
      
   uint16_t P, Q;
   initialize();
   counter++;
   for(uint8_t i = 0; i < 64; ++i)
   {
      P = state[0] & 0x7FC;
      state[1] += T[P / 4];
      state[0] = Bits::rotateRight(state[0], 9);
      state[1] ^= state[0];

      Q = state[1] & 0x7FC;
      state[2] ^= T[Q / 4];
      state[1] = Bits::rotateRight(state[1], 9);
      state[2] += state[1];

      P = (P + state[2]) & 0x7FC;
      state[3] += T[P / 4];
      state[2] = Bits::rotateRight(state[2], 9);
      state[3] ^= state[2];

      Q = (Q + state[3]) & 0x7FC;
      state[0] ^= T[Q / 4];
      state[3] = Bits::rotateRight(state[3], 9);
      state[0] += state[3];


      P = (P + state[0]) & 0x7FC;
      state[1] ^= T[P / 4];
      state[0] = Bits::rotateRight(state[0], 9);

      Q = (Q + state[1]) & 0x7FC;
      state[2] += T[Q / 4];
      state[1] = Bits::rotateRight(state[1], 9);

      P = (P + state[2]) & 0x7FC;
      state[3] ^= T[P / 4];
      state[2] = Bits::rotateRight(state[2], 9);

      Q = (Q + state[3]) & 0x7FC;
      state[0] += T[Q / 4];
      state[3] = Bits::rotateRight(state[3], 9);

      const uint8_t j = 4 * i;         
      keystream.push_back(state[1] + S[j]);
      keystream.push_back(state[2] ^ S[j + 1]);
      keystream.push_back(state[3] + S[j + 2]);
      keystream.push_back(state[0] ^ S[j + 3]);

      if(i & 1)
      {
         state[0] += state[6];
         state[1] += state[7];
         state[2] ^= state[6];
         state[3] ^= state[7];
      }
      else
      {
         state[0] += state[4];
         state[1] += state[5];
         state[2] ^= state[4];
         state[3] ^= state[5];
      }
   }

   return keystream;
}

const UInt32Vector SEAL::generate()
{
   UInt32Vector random_numbers;
   random_numbers.reserve(output_size);
   const uint8_t number_of_Kb = output_size / 1024;
   while(counter < number_of_Kb)
   {
      const UInt32Vector keystream = generateKeystream();
      random_numbers.insert(random_numbers.end(), keystream.begin(), keystream.end());
   }
   
   return random_numbers;
}