
#include "Wolseley.hpp"

#include "String.hpp"

Wolseley::Wolseley(const KeyType &key)
{
   setAlpha(String::grid_uppercase_fr);
   setKey(key);
}

ClassicalType Wolseley::encode(const ClassicalType &clear_text)
{
   ClassicalType crypted;
   crypted.reserve(clear_text.length());

   KeyType key_alpha = getKey();
   key_alpha.reserve(alpha.length());
   key_alpha.append(alpha);
   const ClassicalType new_alpha(String::makeUniqueChars(key_alpha));

   for (const auto c : clear_text)
   {
      const int8_t pos = 24 - new_alpha.find(c);
      crypted.push_back(new_alpha[pos]);
   }

   return crypted;
}

ClassicalType Wolseley::decode(const ClassicalType &cipher_text)
{
   return encode(cipher_text);
}