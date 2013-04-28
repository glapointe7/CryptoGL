#include "Vigenere.hpp"

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