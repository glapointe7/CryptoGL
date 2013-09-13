#include "RC4.hpp"

#include <algorithm>

#include "exceptions/BadKeyLength.hpp"
#include "exceptions/EmptyKey.hpp"

void RC4::setKey(const BytesVector &key)
{
   if(key.empty())
   {
      throw EmptyKey("Your key is empty or not set.");
   }
   
   if(key.size() > 256)
   {
      throw BadKeyLength("Your key has to be less or equal to 32 bytes length.", key.size());
   }
   
   this->key = key;
}

void RC4::keySetup()
{   
   for(uint16_t i = 0; i < 256; ++i)
   {
      subkeys[i] = i;
   }
   
   uint8_t j;
   const uint8_t key_len = key.size();
   for(uint16_t i = 0; i < 256; ++i)
   {
      j = (key[i % key_len] + subkeys[i] + j) & 0xFF;
      std::swap(subkeys[i], subkeys[j]);
   }
}

const BytesVector RC4::encode(const BytesVector &clear_text)
{
   BytesVector crypted;
   crypted.reserve(clear_text.size());
      
   keySetup();
   
   uint8_t j, k;
   for(const auto byte : clear_text)
   {
      j = (j + 1) & 0xFF;
      k = (subkeys[j] + k) & 0xFF;
      std::swap(subkeys[j], subkeys[k]);
      crypted.push_back(byte ^ subkeys[(subkeys[j] + subkeys[k]) & 0xFF]);
   }
   
   return crypted;
}

const BytesVector RC4::decode(const BytesVector &cipher_text)
{
   return encode(cipher_text);
}