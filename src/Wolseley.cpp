
#include "Wolseley.hpp"

#include "Tools.hpp"
#include "String.hpp"

#include "exceptions/BadChar.hpp"
#include "exceptions/EmptyKey.hpp"

Wolseley::Wolseley()
{
   setAlpha(String::grid_uppercase_fr);
}

void Wolseley::setKey(const std::string &key)
{
   if (key.empty())
   {
      throw EmptyKey("Your key is empty.");
   }

   const char c = badAlphaFound(key);
   if (c != 0)
   {
      throw BadChar("Your key contains at least one character that is not in your alphabet.", c);
   }

   this->key = key;
}

const Wolseley::ClassicalType Wolseley::encode(const ClassicalType &clear_text)
{
   ClassicalType crypted = "";
   crypted.reserve(clear_text.length());

   const std::string key_alpha(key + alpha);
   const ClassicalType new_alpha(removeRepeatedLetters(key_alpha));

   for (const auto c : clear_text)
   {
      const std::string::size_type pos = new_alpha.find(c);
      crypted += new_alpha[24 - pos];
   }

   return crypted;
}

const Wolseley::ClassicalType Wolseley::decode(const ClassicalType &cipher_text)
{
   return encode(cipher_text);
}