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
   generateSubkeys();
}

void RC4::generateSubkeys()
{
   for(uint16_t i = 0; i < 256; ++i)
   {
      subkeys[i] = i;
   }
   
   uint8_t j = 0;
   const uint16_t key_len = key.size();
   for(uint16_t i = 0; i < 256; ++i)
   {
      j += key[i % key_len] + subkeys[i];
      std::swap(subkeys[i], subkeys[j]);
   }
}

const BytesVector RC4::generate()
{
   BytesVector keystream;
   keystream.reserve(output_size);
   
   uint8_t i = 0, j = 0;
   for(uint64_t k = 0; k < output_size; ++k)
   {
      i++;
      j += subkeys[i];
      std::swap(subkeys[i], subkeys[j]);
      keystream.push_back(subkeys[(subkeys[i] + subkeys[j]) & 0xFF]);
   }
   
   return keystream;
}

const BytesVector RC4::encode(const BytesVector &clear_text)
{         
   BytesVector crypted;
   output_size = clear_text.size();
   crypted.reserve(output_size);
   
   const BytesVector keystream = generate();
   for(uint64_t i = 0; i < output_size; ++i)
   {
      crypted.push_back(clear_text[i] ^ keystream[i]);
   }
   
   return crypted;
}