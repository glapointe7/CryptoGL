
#include "Wolseley.hpp"

#include "Tools.hpp"
#include "exceptions/EmptyKey.hpp"

Wolseley::Wolseley()
{
   setAlpha("ABCDEFGHIJKLMNOPQRSTUVXYZ");
}

void Wolseley::setKey(const std::string &key)
{
   if(key.empty())
   {
      throw EmptyKey("Tour key should not be empty or not set.");
   }
   else
   {
      this->key = key;
   }
}

// Encode un message avec le chiffre de Wolseley.

const Wolseley::ClassicalType Wolseley::encode(const ClassicalType &clear_text)
{
   ClassicalType crypted = "";
   crypted.reserve(clear_text.length());

   const std::string key_alpha(key + alpha);
   const ClassicalType new_alpha(removeRepeatedLetters(key_alpha));

   for (auto c : clear_text)
   {
      const std::string::size_type pos = new_alpha.find(c);
      crypted += new_alpha[24 - pos];
   }

   return crypted;
}

// Décode un cryptogramme chiffré avec Wolseley.

const Wolseley::ClassicalType Wolseley::decode(const ClassicalType &cipher_text)
{
   return encode(cipher_text);
}