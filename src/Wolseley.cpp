
#include "Wolseley.hpp"

#include "Tools.hpp"
#include "String.hpp"

Wolseley::Wolseley(const KeyType &key)
{
   setAlpha(String::grid_uppercase_fr);
   setKey(key);
}

const Wolseley::ClassicalType Wolseley::encode(const ClassicalType &clear_text)
{
   ClassicalType crypted;
   crypted.reserve(clear_text.length());

   const std::string key_alpha(getKey() + alpha);
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