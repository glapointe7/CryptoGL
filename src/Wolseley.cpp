#include "Wolseley.hpp"

#include "String.hpp"

Wolseley::Wolseley(const KeyType &key)
{
   setAlpha(ClassicalType::grid_uppercase_fr);
   setKey(key);
}

ClassicalType Wolseley::encode(const ClassicalType &clear_text)
{
   ClassicalType new_alpha(getKey().append(alpha));
   new_alpha.removeDuplicates();
   ClassicalType crypted(clear_text.length());
   for (const auto c : clear_text)
   {
      const uint8_t pos = 24 - new_alpha.find(c);
      crypted.push_back(new_alpha[pos]);
   }

   return crypted;
}

ClassicalType Wolseley::decode(const ClassicalType &cipher_text)
{
   return encode(cipher_text);
}