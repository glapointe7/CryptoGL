#include "IDEA.hpp"

#include "Tools.hpp"
#include "MathematicalTools.hpp"

#include "exceptions/BadKeyLength.hpp"

void IDEA::setKey(const BytesContainer &key)
{
   const uint8_t key_len = key.size();
   if (key_len != 16)
   {
      throw BadKeyLength("Your key has to be 128 bits length.", key_len);
   }

   this->key = key;
}

void IDEA::generateSubkeys()
{
   subkeys.reserve(52);
   for (uint8_t j = 0; j < 16; j += 2)
   {
      subkeys.push_back((key[j] << 8) | key[j + 1]);
   }

   // The key is rotated left of 25 bits.
   UInt16Container::iterator it = subkeys.begin();
   for (uint8_t i = 8, j = 0; i < 52; ++i)
   {
      j++;
      subkeys.push_back((*(it + (j & 7)) << 9) | (*(it + ((j + 1) & 7)) >> 7));
      it += j & 8;
      j &= 7;
   }
}

void IDEA::generateInverseSubkeys()
{
   generateSubkeys();
   decoded_subkeys.resize(52);

   decoded_subkeys[51] = inverseMultiplyShort(subkeys[3]);
   decoded_subkeys[50] = -subkeys[2];
   decoded_subkeys[49] = -subkeys[1];
   decoded_subkeys[48] = inverseMultiplyShort(subkeys[0]);

   uint8_t k2 = 47;
   for (uint8_t k1 = 4; k1 < 46; k1 += 6)
   {
      decoded_subkeys[k2--] = subkeys[k1 + 1];
      decoded_subkeys[k2--] = subkeys[k1];
      decoded_subkeys[k2--] = inverseMultiplyShort(subkeys[k1 + 5]);
      decoded_subkeys[k2--] = -subkeys[k1 + 3];
      decoded_subkeys[k2--] = -subkeys[k1 + 4];
      decoded_subkeys[k2--] = inverseMultiplyShort(subkeys[k1 + 2]);
   }
   decoded_subkeys[5] = subkeys[47];
   decoded_subkeys[4] = subkeys[46];
   decoded_subkeys[3] = inverseMultiplyShort(subkeys[51]);
   decoded_subkeys[2] = -subkeys[50];
   decoded_subkeys[1] = -subkeys[49];
   decoded_subkeys[0] = inverseMultiplyShort(subkeys[48]);
}

const IDEA::UInt16Container IDEA::getIntegersFromInputBlock(const BytesContainer &block) const
{
   UInt16Container int_block(4, 0);
   for (uint8_t i = 0; i < 8; i += 2)
   {
      int_block[i >> 1] = (block[i] << 8) | block[i + 1];
   }
   
   return int_block;
}

const IDEA::UInt16Container IDEA::encodeBlock(const UInt16Container &input)
{
   UInt16Container encoded_block(input);
   for (uint8_t k = 0; k < 48; k += 6)
   {
      encoded_block[0] = multiplyShort(encoded_block[0], subkeys[k]);
      encoded_block[1] = addShort(encoded_block[1], subkeys[k + 1]);
      encoded_block[2] = addShort(encoded_block[2], subkeys[k + 2]);
      encoded_block[3] = multiplyShort(encoded_block[3], subkeys[k + 3]);
      const uint16_t t0 = multiplyShort(subkeys[k + 4], encoded_block[0] ^ encoded_block[2]);
      const uint16_t t1 = multiplyShort(subkeys[k + 5], addShort(t0, encoded_block[1] ^ encoded_block[3]));
      const uint16_t t2 = addShort(t0, t1);
      encoded_block[0] ^= t1;
      encoded_block[3] ^= t2;
      const uint16_t a = encoded_block[1] ^ t2;
      encoded_block[1] = encoded_block[2] ^ t1;
      encoded_block[2] = a;
   }

   // Half last round completing the encryption / decryption.
   UInt16Container out(4, 0);
   out[0] = multiplyShort(encoded_block[0], subkeys[48]);
   out[1] = addShort(encoded_block[2], subkeys[49]);
   out[2] = addShort(encoded_block[1], subkeys[50]);
   out[3] = multiplyShort(encoded_block[3], subkeys[51]);
   
   return out;
}

const IDEA::UInt16Container IDEA::decodeBlock(const UInt16Container &input)
{
   subkeys = decoded_subkeys;
   
   return encodeBlock(input);
}

const IDEA::BytesContainer IDEA::getOutputBlock(const UInt16Container &int_block)
{
   BytesContainer output_block;
   output_block.reserve(8);
   for (uint8_t i = 0; i < 4; ++i)
   {
      output_block.push_back((int_block[i] >> 8) & 0xFF);
      output_block.push_back(int_block[i] & 0xFF);
   }

   return output_block;
}