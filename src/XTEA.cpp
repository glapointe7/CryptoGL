#include "XTEA.hpp"

#include "exceptions/BadKeyLength.hpp"

#include "Tools.hpp"
#include "BigEndian.hpp"

void XTEA::setKey(const BytesContainer &key)
{
   const uint8_t key_len = key.size();
   if (key_len != 16)
   {
      throw BadKeyLength("Your key has to be 16 bytes length.", key_len);
   }

   this->key = key;
}

void XTEA::generateSubkeys()
{
   BigEndian32 *LE = new BigEndian32();
   uint32_t tmp_key[4];
   for (uint8_t i = 0; i < 16; i += 4)
   {
      LE->toInteger(BytesContainer(key.begin() + i, key.begin() + i + 4));
      tmp_key[i >> 2] = LE->getValue();
      LE->resetValue();
   }
   delete LE;

   uint32_t sum = 0;
   subkeys.resize(64);
   for (uint8_t i = 0; i < 32; ++i)
   {
      subkeys[i] = (sum + tmp_key[sum & 3]) & 0xFFFFFFFF;
      sum += delta;
      subkeys[32 + i] = (sum + tmp_key[(sum >> 11) & 3]) & 0xFFFFFFFF;
   }
}

uint64_t XTEA::F(const uint64_t &X, const uint64_t &subkey) const
{
   return (((X << 4) ^ (X >> 5)) + X) ^ subkey;
}

void XTEA::encodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const
{
   for (uint8_t i = 0; i < rounds; ++i)
   {
      L = (L + F(R, subkeys[i])) & 0xFFFFFFFF;
      R = (R + F(L, subkeys[i + 32])) & 0xFFFFFFFF;
   }
}

void XTEA::decodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const
{
   for (int8_t i = rounds - 1; i >= 0; --i)
   {
      R = (R - F(L, subkeys[i + 32])) & 0xFFFFFFFF;
      L = (L - F(R, subkeys[i])) & 0xFFFFFFFF;
   }
}

const XTEA::BytesContainer XTEA::getOutputBlock(const BytesContainer &block, const bool to_encode)
{
   BigEndian32 *LE = new BigEndian32();
   LE->toInteger(BytesContainer(block.begin(), block.begin() + 4));
   uint64_t L = LE->getValue();
   LE->resetValue();

   LE->toInteger(BytesContainer(block.begin() + 4, block.end()));
   uint64_t R = LE->getValue();
   delete LE;

   if (to_encode)
   {
      encodeFeistelRounds(L, R, 0);
   }
   else
   {
      decodeFeistelRounds(L, R, 0);
   }
   
   // Convert 32-bit integers to 8-bit integers output.
   BytesContainer output;
   output.reserve(8);
   
   BigEndian32 *LE_32 = new BigEndian32();
   LE_32->toBytes(L);
   BytesContainer tmp = LE_32->getBytes();
   output.insert(output.end(), tmp.begin(), tmp.end());
   
   LE_32->toBytes(R);
   tmp = LE_32->getBytes();
   output.insert(output.end(), tmp.begin(), tmp.end());
   delete LE_32;

   return output;
}

const XTEA::BytesContainer XTEA::encode(const BytesContainer &clear_text)
{
   return processEncoding(clear_text);
}

const XTEA::BytesContainer XTEA::decode(const BytesContainer &cipher_text)
{
   return processDecoding(cipher_text);
}