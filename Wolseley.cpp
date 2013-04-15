
#include "Wolseley.h"

#include <algorithm>

#include "Tools.h"

Wolseley::Wolseley()
{
   setAlpha("ABCDEFGHIJKLMNOPQRSTUVXYZ");
}

void Wolseley::setKey(const std::string &key)
{
   this->key = key;
}

// Encode un message avec le chiffre de Wolseley.

const Wolseley::ClassicalType Wolseley::encode(const ClassicalType &clear_text)
{
   ClassicalType crypted = "";
   crypted.reserve(clear_text.length());

   std::string key_alpha(key + alpha);
   ClassicalType new_alpha(removeRepeatedLetters(key_alpha));

   for (auto c : clear_text)
   {
      std::string::size_type pos = new_alpha.find(c);
      crypted += new_alpha[24 - pos];
   }

   return crypted;
}

// Décode un cryptogramme chiffré avec Wolseley.

const Wolseley::ClassicalType Wolseley::decode(const ClassicalType &cipher_text)
{
   ClassicalType decrypted = "";
   decrypted.reserve(cipher_text.length());

   std::string key_alpha(key + alpha);
   ClassicalType new_alpha(removeRepeatedLetters(key_alpha));

   for (auto c : cipher_text)
   {
      std::string::size_type pos = new_alpha.find(c);
      decrypted += new_alpha[24 - pos];
   }
  
   return decrypted;
}