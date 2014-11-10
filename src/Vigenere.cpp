#include "Vigenere.hpp"

using namespace CryptoGL;

// Process the core of the Vigenere algorithm
ClassicalType Vigenere::process(const ClassicalType &text, const GetCharFunction &getNextChar)
{
   const KeyType my_key = getKey();
   const uint32_t key_length = my_key.length();

   ClassicalType toReturn(text.length());

   uint32_t idx = 0;
   for (const auto c : text)
   {
      toReturn.append(getNextChar(alpha, c, my_key[idx]));
      idx = (idx + 1) % key_length;
   }

   return toReturn;
}

ClassicalType Vigenere::encode(const ClassicalType &clear_text)
{
   return process(clear_text, charEncode);
}

ClassicalType Vigenere::decode(const ClassicalType &cipher_text)
{
   return process(cipher_text, charDecode);
}