#include "RC4.hpp"

#include <algorithm>

#include "exceptions/BadKeyLength.hpp"
#include "exceptions/EmptyKey.hpp"

RC4::RC4(const BytesContainer &key)
{
   if(key.empty())
   {
      throw EmptyKey("Your RC4 key is empty or not set.");
   }
   
   // The key have to be <= than 32 bytes.
   if(key.size() > 32)
   {
      throw BadKeyLength("Your RC4 key length have to be less or equal to 32 bytes.", key.size());
   }
   
   this->key = key;
   
   // Initialize 'state'.
   for(uint16_t i = 0; i < 256; ++i)
   {
      state[i] = i;
   }
}

void RC4::keySetup()
{
   const uint8_t key_len = key.size();
   uint8_t j = 0;
   for(uint16_t i = 0; i < 256; ++i)
   {
      j = (key[i % key_len] + state[i] + j) & 0xFF;
      std::swap(state[i], state[j]);
   }
}

const RC4::BytesContainer RC4::encode(const BytesContainer &clear_text)
{
   BytesContainer crypted;
   crypted.reserve(clear_text.size());
   
   keySetup();
   
   uint8_t j = 0;
   uint8_t k = 0;
   for(const auto byte : clear_text)
   {
      j = (j + 1) & 0xFF;
      k = (state[j] + k) & 0xFF;
      std::swap(state[j], state[k]);
      crypted.push_back(byte ^ state[(state[j] + state[k]) & 0xFF]);
   }
   
   return crypted;
}

const RC4::BytesContainer RC4::decode(const BytesContainer &cipher_text)
{
   return encode(cipher_text);
}