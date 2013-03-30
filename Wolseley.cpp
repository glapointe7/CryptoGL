
#include "Wolseley.h"

#include <algorithm>

#include "Data.h"
#include "Tools.h"

Wolseley::Wolseley()
{
   clear_text = Data::load("clear_text.txt");
   cipher_text = Data::load("cipher_text.txt");
   //transform(clear_text.begin(), clear_text.end(), clear_text.begin(), ::toupper);
   setAlpha("ABCDEFGHIJKLMNOPQRSTUVXYZ");
}

void Wolseley::setKey(const string &key)
{
   this->key = key;
}

// Encode un message avec le chiffre de Wolseley.

string Wolseley::encode(const std::string &)
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
   Data::save("cipher_text.txt", crypted);

   return crypted;
}

// Décode un cryptogramme chiffré avec Wolseley.

string Wolseley::decode(const std::string &)
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
   Data::save("clear_text.txt", decrypted);

   return decrypted;
}