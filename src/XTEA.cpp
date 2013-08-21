#include "XTEA.hpp"

#include "exceptions/BadKeyLength.hpp"

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
   BigEndian32 BE;
   uint32_t tmp_key[4];
   for (uint8_t i = 0; i < 16; i += 4)
   {
      BE.toInteger(BytesContainer(key.begin() + i, key.begin() + i + 4));
      tmp_key[i >> 2] = BE.getValue();
      BE.resetValue();
   }

   uint32_t sum = 0;
   subkeys.resize(64);
   for (uint8_t i = 0; i < 32; ++i)
   {
      subkeys[i] = sum + tmp_key[sum & 3];
      sum += delta;
      subkeys[32 + i] = sum + tmp_key[(sum >> 11) & 3];
   }
}

uint32_t XTEA::F(const uint32_t half_block, const uint32_t subkey) const
{
   return (((half_block << 4) ^ (half_block >> 5)) + half_block) ^ subkey;
}

void XTEA::encodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const
{
   for (uint8_t i = 0; i < rounds; ++i)
   {
      L += F(R, subkeys[i]);
      R += F(L, subkeys[i + 32]);
   }
}

void XTEA::decodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const
{
   for (int8_t i = rounds - 1; i >= 0; --i)
   {
      R -= F(L, subkeys[i + 32]) ;
      L -= F(R, subkeys[i]);
   }
}

const XTEA::UInt32Container XTEA::getIntegersFromInputBlock(const BytesContainer &block) const
{
   UInt32Container int_block;
   int_block.reserve(2);
   BigEndian32 BE;
   for(uint8_t i = 0; i < 8; i += 4)
   {
      BE.toInteger(BytesContainer(block.begin() + i, block.begin() + i + 4));
      int_block.push_back(BE.getValue());
      BE.resetValue();
   }
   
   return int_block;
}

const XTEA::UInt32Container XTEA::encodeBlock(const UInt32Container &input)
{
   UInt32Container LR(input);
   encodeFeistelRounds(LR[0], LR[1], 0);
   
   return LR;
}

const XTEA::UInt32Container XTEA::decodeBlock(const UInt32Container &input)
{
   UInt32Container LR(input);
   decodeFeistelRounds(LR[0], LR[1], 0);
   
   return LR;
}

const XTEA::BytesContainer XTEA::getOutputBlock(const UInt32Container &int_block)
{
   BytesContainer output_block;
   output_block.reserve(8);
   
   BigEndian32 BE;
   for(uint8_t i = 0; i < 2; ++i)
   {
      BE.toBytes(int_block[i]);
      const BytesContainer tmp = BE.getBytes();
      output_block.insert(output_block.end(), tmp.begin(), tmp.end());
   }

   return output_block;
}