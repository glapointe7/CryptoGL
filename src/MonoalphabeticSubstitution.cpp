#include "MonoalphabeticSubstitution.hpp"

const ClassicalType MonoalphabeticSubstitution::encode(const ClassicalType &clear_text)
{
   const KeyType key = getKey();
   ClassicalType crypted;
   crypted.reserve(clear_text.length());
   
   for(const auto c : clear_text)
   {
      crypted += key[alpha.find(c)];
   }
   
   return crypted;
}

const ClassicalType MonoalphabeticSubstitution::decode(const ClassicalType &cipher_text)
{
   const KeyType key = getKey();
   ClassicalType decrypted;
   decrypted.reserve(cipher_text.length());
   
   for(const auto c : cipher_text)
   {
      decrypted += alpha[key.find(c)];
   }
   
   return decrypted;
}