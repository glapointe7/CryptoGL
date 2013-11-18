#include "RC4.hpp"

#include <algorithm>

#include "exceptions/BadKeyLength.hpp"

void RC4::setKey(const BytesVector &key)
{
   if(key.size() > 256 || key.size() == 0)
   {
      throw BadKeyLength("Your key has to be less or equal to 32 bytes length and not empty.", key.size());
   }
   
   this->key = key;
   keySetup();
}

void RC4::keySetup()
{
   std::iota(subkeys.begin(), subkeys.end(), 0);
   
   const uint16_t key_len = key.size();
   uint8_t j = 0;
   for(uint16_t i = 0; i < 256; ++i)
   {
      j += key[i % key_len] + subkeys[i];
      std::swap(subkeys[i], subkeys[j]);
   }
}

BytesVector RC4::generateKeystream()
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

BytesVector RC4::encode(const BytesVector &clear_text)
{         
   BytesVector crypted;
   output_size = clear_text.size();
   crypted.reserve(output_size);
   
   const BytesVector keystream = generateKeystream();
   for(uint64_t i = 0; i < output_size; ++i)
   {
      crypted.push_back(clear_text[i] ^ keystream[i]);
   }
   
   return crypted;
}