#include "UnsortAlpha.hpp"

#include "String.hpp"

UnsortAlpha::UnsortAlpha(const KeyType &key)
   : charEncode(sortAlpha), charDecode(unorderedAlpha)
{
   unsort_alpha.reserve(alpha.length());
   setKey(key);
}

void UnsortAlpha::setHorizontalAlpha()
{  
   unsort_alpha = String::makeUniqueChars(getKey().append(alpha));
}

// Build an unordered vertical alphabet by transforming the key with only unique chars. 

void UnsortAlpha::setVerticalAlpha()
{
   const ClassicalType new_key = String::makeUniqueChars(getKey());
   const ClassicalType str = String::makeUniqueChars(new_key + alpha);
   const uint32_t key_len = new_key.length();
   const uint32_t alpha_len = str.length();

   for (uint32_t i = 0; i < key_len; ++i)
   {
      uint32_t j = i;
      while (j < alpha_len)
      {
         unsort_alpha.push_back(str[j]);
         j += key_len;
      }
   }
}

ClassicalType UnsortAlpha::process(const ClassicalType &text, const GetCharFunction &getNextChar)
{
   ClassicalType toReturn;
   toReturn.reserve(text.length());

   for (const auto c : text)
   {
      toReturn.push_back(getNextChar(alpha, unsort_alpha, c));
   }

   return toReturn;
}

ClassicalType UnsortAlpha::encode(const ClassicalType &clear_text)
{
   return process(clear_text, charEncode);
}

ClassicalType UnsortAlpha::decode(const ClassicalType &cipher_text)
{
   return process(cipher_text, charDecode);
}