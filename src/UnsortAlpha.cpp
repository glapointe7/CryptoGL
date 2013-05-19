
#include "UnsortAlpha.hpp"

#include "Tools.hpp"

#include "exceptions/BadChar.hpp"
#include "exceptions/EmptyKey.hpp"

UnsortAlpha::UnsortAlpha()
{
   unsort_alpha.reserve(alpha.length());
}

void UnsortAlpha::setKey(const std::string &key)
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

void UnsortAlpha::setHorizontalAlpha()
{  
   unsort_alpha = removeRepeatedLetters(key + alpha);
}

// Build an unordered vertical alphabet by transforming the key with only unique chars. 

void UnsortAlpha::setVerticalAlpha()
{
   const std::string new_key(removeRepeatedLetters(key));
   const std::string str(removeRepeatedLetters(new_key + alpha));
   const uint32_t key_len = new_key.length();
   const uint32_t alpha_len = str.length();

   for (uint32_t i = 0; i < key_len; i++)
   {
      uint32_t j = i;
      while (j < alpha_len)
      {
         unsort_alpha += str[j];
         j += key_len;
      }
   }
}

const UnsortAlpha::ClassicalType UnsortAlpha::encode(const ClassicalType &clear_text)
{
   ClassicalType crypted = "";
   crypted.reserve(clear_text.length());

   for (const auto c : clear_text)
   {
      crypted += unsort_alpha[alpha.find(c)];
   }

   return crypted;
}

const UnsortAlpha::ClassicalType UnsortAlpha::decode(const ClassicalType &cipher_text)
{
   ClassicalType decrypted = "";
   decrypted.reserve(cipher_text.length());

   for (const auto c : cipher_text)
   {
      decrypted += alpha[unsort_alpha.find(c)];
   }

   return decrypted;
}