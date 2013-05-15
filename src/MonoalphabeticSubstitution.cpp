#include "MonoalphabeticSubstitution.hpp"

#include "exceptions/EmptyKey.hpp"

void MonoalphabeticSubstitution::setKey(const ClassicalType &key)
{
   if(key.empty())
   {
      throw EmptyKey("Tour key have to be not empty.");
   }
   
   this->key = key;
}

const MonoalphabeticSubstitution::ClassicalType 
MonoalphabeticSubstitution::encode(const ClassicalType &clear_text)
{
   ClassicalType crypted = "";
   crypted.reserve(clear_text.length());
   
   for(const auto c : clear_text)
   {
      crypted += key[alpha.find(c)];
   }
   
   return crypted;
}

const MonoalphabeticSubstitution::ClassicalType 
MonoalphabeticSubstitution::decode(const ClassicalType &cipher_text)
{
   ClassicalType decrypted = "";
   decrypted.reserve(cipher_text.length());
   
   for(const auto c : cipher_text)
   {
      decrypted += alpha[key.find(c)];
   }
   
   return decrypted;
}