#include "Noekeon.hpp"

#include <algorithm>

#include "Tools.hpp"
#include "BigEndian.hpp"

#include "exceptions/BadKeyLength.hpp"

constexpr uint32_t Noekeon::round_constants[];

void Noekeon::setKey(const BytesContainer &key)
{
   const uint8_t key_len = key.size();
   if (key_len != 16)
   {
      throw BadKeyLength("Your key has to be 128 bits length.", key_len);
   }

   this->key = key;
}

void Noekeon::generateSubkeys()
{
   subkeys.resize(4);
   BigEndian32 BE;
   for(uint8_t i = 0; i < 4; ++i)
   {
      BE.toInteger(BytesContainer(key.begin() + (i << 2), key.begin() + (i << 2) + 4));
      subkeys[i] = BE.getValue();
      BE.resetValue();
   }
}

void Noekeon::generateInverseSubkeys()
{
   generateSubkeys();
   applyTheta();
}

void Noekeon::applyGamma(UInt32Container &state)
{
   state[1] ^= ~(state[3] | state[2]);
   state[0] ^= state[2] & state[1];
   
   std::swap(state[3], state[0]);
   state[2] ^= state[0] ^ state[1] ^ state[3];
   
   state[1] ^= ~(state[3] | state[2]);
   state[0] ^= state[2] & state[1];
}

void Noekeon::applyTheta()
{
   uint32_t temp = subkeys[0] ^ subkeys[2];
   temp ^= rotateRight(temp, 8, 32) ^ rotateLeft(temp, 8, 32);
   subkeys[1] ^= temp;
   subkeys[3] ^= temp;
   
   temp = subkeys[1] ^ subkeys[3];
   temp ^= rotateRight(temp, 8, 32) ^ rotateLeft(temp, 8, 32);
   subkeys[0] ^= temp;
   subkeys[2] ^= temp;
}

void Noekeon::applyTheta(UInt32Container &state) const
{
   uint32_t temp = state[0] ^ state[2];
   temp ^= rotateRight(temp, 8, 32) ^ rotateLeft(temp, 8, 32);
   state[1] ^= temp;
   state[3] ^= temp;
   
   for(uint8_t i = 0; i < 4; ++i)
   {
      state[i] ^= subkeys[i];
   }
   
   temp = state[1] ^ state[3];
   temp ^= rotateRight(temp, 8, 32) ^ rotateLeft(temp, 8, 32);
   state[0] ^= temp;
   state[2] ^= temp;
}

void Noekeon::applyPi1(UInt32Container &state)
{
   state[1] = rotateLeft(state[1], 1, 32);
   state[2] = rotateLeft(state[2], 5, 32);
   state[3] = rotateLeft(state[3], 2, 32);
}

void Noekeon::applyPi2(UInt32Container &state)
{
   state[1] = rotateRight(state[1], 1, 32);
   state[2] = rotateRight(state[2], 5, 32);
   state[3] = rotateRight(state[3], 2, 32);
}

void Noekeon::applyRound(UInt32Container &state, const uint8_t constant1, const uint8_t constant2)
{
   state[0] ^= constant1;
   applyTheta(state);
   state[0] ^= constant2;
   applyPi1(state);
   applyGamma(state);
   applyPi2(state);
}

const Noekeon::UInt32Container Noekeon::getIntegersFromInputBlock(const BytesContainer &block) const
{
   BigEndian32 BE;
   UInt32Container int_block(4, 0);
   for(uint8_t i = 0; i < 4; ++i)
   {
      BE.toInteger(BytesContainer(block.begin() + (i << 2), block.begin() + (i << 2) + 4));
      int_block[i] = BE.getValue();
      BE.resetValue();
   }
   
   return int_block;
}

const Noekeon::UInt32Container Noekeon::encodeBlock(const UInt32Container &input)
{
   UInt32Container state(input);
   for(uint8_t i = 0; i < number_of_rounds; ++i)
   {
      applyRound(state, round_constants[i], 0);
   }
   state[0] ^= round_constants[number_of_rounds];
   applyTheta(state);
   
   return state;
}

const Noekeon::UInt32Container Noekeon::decodeBlock(const UInt32Container &input)
{
   UInt32Container state(input);
   for(uint8_t i = number_of_rounds; i > 0; --i)
   {
      applyRound(state, 0, round_constants[i]);
   }
   applyTheta(state);
   state[0] ^= round_constants[0];
   
   return state;
}

const Noekeon::BytesContainer Noekeon::getOutputBlock(const UInt32Container &int_block)
{
   BytesContainer output_block;
   output_block.reserve(16);
   
   BigEndian32 BE;
   for(uint8_t i = 0; i < 4; ++i)
   {
      BE.toBytes(int_block[i]);
      const BytesContainer tmp = BE.getBytes();
      output_block.insert(output_block.end(), tmp.begin(), tmp.end());
   }

   return output_block;
}