
#include "UnsortAlpha.hpp"

#include "Tools.hpp"

UnsortAlpha::UnsortAlpha(const KeyType &key)
   : charEncode(sortAlpha), charDecode(unorderedAlpha)
{
   unsort_alpha.reserve(alpha.length());
   setKey(key);
}

void UnsortAlpha::setHorizontalAlpha()
{  
   unsort_alpha = removeRepeatedLetters(getKey() + alpha);
}

// Build an unordered vertical alphabet by transforming the key with only unique chars. 

void UnsortAlpha::setVerticalAlpha()
{
   const ClassicalType new_key(removeRepeatedLetters(getKey()));
   const ClassicalType str(removeRepeatedLetters(new_key + alpha));
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

const ClassicalType 
UnsortAlpha::process(const ClassicalType &text, const GetCharFunction &getNextChar)
{
   ClassicalType toReturn;
   toReturn.reserve(text.length());

   for (const auto c : text)
   {
      toReturn.push_back(getNextChar(alpha, unsort_alpha, c));
   }

   return toReturn;
}

const ClassicalType UnsortAlpha::encode(const ClassicalType &clear_text)
{
   return process(clear_text, charEncode);
}

const ClassicalType UnsortAlpha::decode(const ClassicalType &cipher_text)
{
   return process(cipher_text, charDecode);
}