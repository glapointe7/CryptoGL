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
   encoded_subkeys.reserve(52);
   for (uint8_t j = 0; j < 16; j += 2)
   {
      encoded_subkeys.push_back((key[j] << 8) | key[j + 1]);
   }

   // The key is rotated left of 25 bits.
   UInt16Container::iterator it = encoded_subkeys.begin();
   for (uint8_t i = 8, j = 0; i < 52; ++i)
   {
      j++;
      encoded_subkeys.push_back((*(it + (j & 7)) << 9) | (*(it + ((j + 1) & 7)) >> 7));
      it += j & 8;
      j &= 7;
   }
}

void IDEA::generateInverseSubkeys()
{
   generateSubkeys();
   decoded_subkeys.resize(52);

   decoded_subkeys[51] = inverseMultiplyShort(encoded_subkeys[3]);
   decoded_subkeys[50] = -encoded_subkeys[2];
   decoded_subkeys[49] = -encoded_subkeys[1];
   decoded_subkeys[48] = inverseMultiplyShort(encoded_subkeys[0]);

   uint8_t k2 = 47;
   for (uint8_t k1 = 4; k1 < 46; k1 += 6)
   {
      decoded_subkeys[k2--] = encoded_subkeys[k1 + 1];
      decoded_subkeys[k2--] = encoded_subkeys[k1];
      decoded_subkeys[k2--] = inverseMultiplyShort(encoded_subkeys[k1 + 5]);
      decoded_subkeys[k2--] = -encoded_subkeys[k1 + 3];
      decoded_subkeys[k2--] = -encoded_subkeys[k1 + 4];
      decoded_subkeys[k2--] = inverseMultiplyShort(encoded_subkeys[k1 + 2]);
   }
   decoded_subkeys[5] = encoded_subkeys[47];
   decoded_subkeys[4] = encoded_subkeys[46];
   decoded_subkeys[3] = inverseMultiplyShort(encoded_subkeys[51]);
   decoded_subkeys[2] = -encoded_subkeys[50];
   decoded_subkeys[1] = -encoded_subkeys[49];
   decoded_subkeys[0] = inverseMultiplyShort(encoded_subkeys[48]);
}

const IDEA::BytesContainer IDEA::getOutputBlock(const BytesContainer &block, const bool to_encode)
{
   // Convert from 8-bit block to 16-bit.
   uint16_t input[4];
   for (uint8_t i = 0; i < 8; i += 2)
   {
      input[i >> 1] = (block[i] << 8) | block[i + 1];
   }

   if (!to_encode)
   {
      encoded_subkeys = decoded_subkeys;
   }

   for (uint8_t k = 0; k < 48; k += 6)
   {
      input[0] = multiplyShort(input[0], encoded_subkeys[k]);
      input[1] = addShort(input[1], encoded_subkeys[k + 1]);
      input[2] = addShort(input[2], encoded_subkeys[k + 2]);
      input[3] = multiplyShort(input[3], encoded_subkeys[k + 3]);
      const uint16_t t0 = multiplyShort(encoded_subkeys[k + 4], input[0] ^ input[2]);
      const uint16_t t1 = multiplyShort(encoded_subkeys[k + 5], addShort(t0, input[1] ^ input[3]));
      const uint16_t t2 = addShort(t0, t1);
      input[0] ^= t1;
      input[3] ^= t2;
      const uint16_t a = input[1] ^ t2;
      input[1] = input[2] ^ t1;
      input[2] = a;
   }

   // Half last round completing the encryption / decryption.
   uint16_t out[4];
   out[0] = multiplyShort(input[0], encoded_subkeys[48]);
   out[1] = addShort(input[2], encoded_subkeys[49]);
   out[2] = addShort(input[1], encoded_subkeys[50]);
   out[3] = multiplyShort(input[3], encoded_subkeys[51]);

   // Convert from 16-bit vector to 8-bit vector.
   BytesContainer output;
   output.reserve(8);
   for (uint8_t i = 0; i < 4; ++i)
   {
      output.push_back((out[i] >> 8) & 0xFF);
      output.push_back(out[i] & 0xFF);
   }

   return output;
}

const IDEA::BytesContainer IDEA::encode(const BytesContainer &clear_text)
{
   return process(addPadding(clear_text, 8, 0), 8, true);
}

const IDEA::BytesContainer IDEA::decode(const BytesContainer &cipher_text)
{
   return process(cipher_text, 8, false);
}