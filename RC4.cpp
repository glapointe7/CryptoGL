#include "RC4.hpp"

#include <algorithm>

void RC4::setKey(const BytesContainer &key)
{
   this->key = key;
}

void RC4::initState()
{
   state.reserve(256);
   for(unsigned short i = 0; i < 256; ++i)
   {
      state.push_back(i);
   }
}

// La clé ne doit pas dépasser 256 octets.
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

RC4::BytesContainer RC4::encode(const BytesContainer &clear_text)
{
   unsigned int clear_len = clear_text.size();
   BytesContainer crypted;
   crypted.reserve(clear_len);
   
   initState();
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

RC4::BytesContainer RC4::decode(const BytesContainer &cipher_text)
{
   unsigned int cipher_len = cipher_text.size();
   BytesContainer decrypted;
   decrypted.reserve(cipher_len);
   
   return decrypted;
}