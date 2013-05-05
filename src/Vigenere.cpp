#include "Vigenere.hpp"

void Vigenere::setKey(const Key &key)
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

// Exécution de l'algorithme principal de Vigenere et de ses variantes.

const Vigenere::ClassicalType
Vigenere::process(const ClassicalType &text, const GetCharFunction &getNextChar)
{
   const unsigned int key_length = key.length();

   ClassicalType toReturn = "";
   toReturn.reserve(text.length());

   unsigned int idx = 0;
   for (auto c : text)
   {
      toReturn += getNextChar(alpha, c, key[idx]);
      idx = (idx + 1) % key_length;
   }

   return toReturn;
}

const Vigenere::ClassicalType Vigenere::encode(const ClassicalType &clear_text)
{
   if (key.empty())
   {
      throw EmptyKey("Your key is not set.");
   }

   return process(clear_text, charEncode);
}

const Vigenere::ClassicalType Vigenere::decode(const ClassicalType &cipher_text)
{
   if (key.empty())
   {
      throw EmptyKey("Your key is not set.");
   }

   return process(cipher_text, charDecode);
}