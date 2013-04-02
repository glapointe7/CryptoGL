
#include "Wolseley.h"

#include <algorithm>

#include "Tools.h"

Wolseley::Wolseley()
{
   setAlpha("ABCDEFGHIJKLMNOPQRSTUVXYZ");
}

void Wolseley::setKey(const string &key)
{
   this->key = key;
}

// Encode un message avec le chiffre de Wolseley.

string Wolseley::encode(const std::string &clear_text)
{
   string crypted = "";
   crypted.reserve(clear_text.length());

   string key_alpha(key + alpha);
   string new_alpha(removeRepeatedLetters(key_alpha));

   for (auto c : clear_text)
   {
      string::size_type pos = new_alpha.find(c);
      crypted += new_alpha[24 - pos];
   }

   return crypted;
}

// Décode un cryptogramme chiffré avec Wolseley.

string Wolseley::decode(const std::string &cipher_text)
{
   string decrypted = "";
   decrypted.reserve(cipher_text.length());

   string key_alpha(key + alpha);
   string new_alpha(removeRepeatedLetters(key_alpha));

   for (auto c : cipher_text)
   {
      string::size_type pos = new_alpha.find(c);
      decrypted += new_alpha[24 - pos];
   }
  
   return decrypted;
}