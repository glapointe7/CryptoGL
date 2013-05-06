#include "RC4.hpp"

#include <algorithm>

#include "exceptions/BadKeyLength.hpp"
#include "exceptions/EmptyKey.hpp"

// La clé ne doit pas dépasser 256 octets.
void RC4::setKey(const BytesContainer &key)
{
   if(key.empty())
   {
      throw EmptyKey("Your RC4 key is empty or not set.");
   }
   
   if(key.size() > 32)
   {
      throw BadKeyLength("Your RC4 key length is greater than 32 bytes.", key.size());
   }
   
   this->key = key;
}

void RC4::initialize()
{
   state.reserve(256);
   for(unsigned short i = 0; i < 256; ++i)
   {
      state.push_back(i);
   }
}


void RC4::keySetup()
{
   unsigned char key_len = key.size();
   unsigned char j = 0;
   for(unsigned short i = 0; i < 256; ++i)
   {
      j = (key[i % key_len] + state[i] + j) & 0xFF;
      std::swap(state[i], state[j]);
   }
}

const RC4::BytesContainer RC4::encode(const BytesContainer &clear_text)
{
   BytesContainer crypted;
   crypted.reserve(clear_text.size());
   
   initialize();
   keySetup();
   
   unsigned char j = 0;
   unsigned char k = 0;
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