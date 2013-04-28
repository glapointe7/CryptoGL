#include "RC4.hpp"

#include <algorithm>

#include "BadKeyLength.hpp"
#include "EmptyKey.hpp"

// La clé ne doit pas dépasser 256 octets.
void RC4::setKey(const BytesContainer &key)
{
   if(key.empty())
   {
      throw EmptyKey("Your RC4 key is empty or not set.");
   }
   else if(key.size() > 32)
   {
      throw BadKeyLength("Your RC4 key length should not be greater than 32 bytes.", key.size());
   }
   else
   {
      this->key = key;
   }
}

void RC4::initialize()
{
   state.reserve(256);
   for(unsigned short i = 0; i < 256; ++i)
   {
      state.push_back(i);
   }
}


void RC4::initKeySchedule()
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
   unsigned int clear_len = clear_text.size();
   BytesContainer crypted;
   crypted.reserve(clear_len);
   
   initialize();
   initKeySchedule();
   
   unsigned char j = 0;
   unsigned char k = 0;
   for(auto byte : clear_text)
   {
      j = (j + 1) & 0xFF;
      k = (state[j] + k) & 0xFF;
      std::swap(state[j], state[k]);
      crypted.push_back(byte ^ state[(state[j] + state[k]) & 0xFF]);
   }
   
   return crypted;
}

// TODO
const RC4::BytesContainer RC4::decode(const BytesContainer &cipher_text)
{
   return encode(cipher_text);
}