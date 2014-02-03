#include "Noekeon.hpp"

#include <algorithm>

#include "Bits.hpp"
#include "Endian.hpp"

#include "exceptions/BadKeyLength.hpp"

constexpr std::array<uint32_t, 17> Noekeon::round_constants;

void Noekeon::setKey(const BytesVector &key)
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
   subkeys = BigEndian32::toIntegersVector(key);
}

void Noekeon::generateInverseSubkeys()
{
   generateSubkeys();
   applyThetaToSubkeys();
}

void Noekeon::applyGamma()
{
   state[1] ^= ~(state[3] | state[2]);
   state[0] ^= state[2] & state[1];
   
   std::swap(state[3], state[0]);
   state[2] ^= state[0] ^ state[1] ^ state[3];
   
   state[1] ^= ~(state[3] | state[2]);
   state[0] ^= state[2] & state[1];
}

void Noekeon::setClassVector(const std::array<uint8_t, 4> &indices, UInt32Vector &V)
{
   uint32_t temp = V[indices[0]] ^ V[indices[1]];
   temp ^= Bits::rotateRight(temp, 8) ^ Bits::rotateLeft(temp, 8);
   V[indices[2]] ^= temp;
   V[indices[3]] ^= temp;
}

void Noekeon::applyThetaToSubkeys()
{
   setClassVector({{0, 2, 1, 3}}, subkeys);
   setClassVector({{1, 3, 0, 2}}, subkeys);
}

void Noekeon::applyThetaToState()
{
   setClassVector({{0, 2, 1, 3}}, state);
   
   for(uint8_t i = 0; i < 4; ++i)
   {
      state[i] ^= subkeys[i];
   }
   
   setClassVector({{1, 3, 0, 2}}, state);
}

void Noekeon::applyPiLeft()
{
   state[1] = Bits::rotateLeft(state[1], 1);
   state[2] = Bits::rotateLeft(state[2], 5);
   state[3] = Bits::rotateLeft(state[3], 2);
}

void Noekeon::applyPiRight()
{
   state[1] = Bits::rotateRight(state[1], 1);
   state[2] = Bits::rotateRight(state[2], 5);
   state[3] = Bits::rotateRight(state[3], 2);
}

void Noekeon::applyRound(const uint8_t constant1, const uint8_t constant2)
{
   state[0] ^= constant1;
   applyThetaToState();
   state[0] ^= constant2;
   applyPiLeft();
   applyGamma();
   applyPiRight();
}

UInt32Vector Noekeon::encodeBlock(const UInt32Vector &input)
{
   state = input;
   for(uint8_t i = 0; i < rounds; ++i)
   {
      applyRound(round_constants[i], 0);
   }
   state[0] ^= round_constants[rounds];
   applyThetaToState();
   
   return state;
}

UInt32Vector Noekeon::decodeBlock(const UInt32Vector &input)
{
   state = input;
   for(uint8_t i = rounds; i > 0; --i)
   {
      applyRound(0, round_constants[i]);
   }
   applyThetaToState();
   state[0] ^= round_constants[0];
   
   return state;
}