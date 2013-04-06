#include "converterTools.h"
#include <bitset>
#include <sstream>
#include <algorithm>
#include <vector>

// Convertit chaque Majuscule et minuscule en un symbole quelconque du code ASCII.
// symbol doit contenir 2 caract�res : 0 = MAJUSCULE  1 = minuscule.

void convertMajMinToSymbol(std::string &text, const std::string symbol)
{
   // On remplace chaque caractère en majuscule par symbol[0].
   replace_if(text.begin(), text.end(), [](char c) {
      return (isupper(c));
   }, symbol[0]);

   // On remplace chaque caractère en minuscule par symbol[1].
   replace_if(text.begin(), text.end(), [](char c) {
      return (islower(c));
   }, symbol[1]);
}

// Convertit un string en une suite binaire.

std::string convertTextToBinaryString(const std::string &text)
{
   std::string bits = "";

   for (auto c : text)
   {
      std::bitset<8> byte(c);
      bits += byte.to_string();
   }

   return bits;
}

// Convertit un texte en majuscules.

void uppercase(std::string &text)
{
   transform(text.begin(), text.end(), text.begin(), ::toupper);
}

// Transforme un String en un nombre.

template <class Type>
Type lexical_cast(const std::string &textNumber)
{
   std::istringstream ss(textNumber);

   Type number;
   ss >> number;

   return number;
}

/**
 * Convertit une string en base from_base vers une string en base to_base.
 *
 * @param string number : Le nombre à convertir.
 * @param int from_base : La base de number au départ.
 * @param int to_base : La base pour laquelle number doit être converti.
 * @return string result
 */
std::string baseConvertString(const std::string &number, const unsigned char from_base,
        const unsigned char to_base)
{
   std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_";
   std::string result = "";
   std::vector<unsigned char> pos;

   for (auto c : number)
   {
      pos.push_back(chars.find(c));
   }

   unsigned int length = number.length();
   unsigned int newlen = 0;
   do
   {
      uint_fast64_t divide = 0;
      for (unsigned int i = 0; i < length; i++)
      { // Perform division manually (which is why this works with big numbers)
         divide = divide * from_base + pos[i];
         if (divide >= to_base)
         {
            pos[newlen++] = static_cast<int> (divide / to_base);
            divide %= to_base;
         }
         else if (newlen > 0)
         {
            pos[newlen++] = 0;
         }
      }
      length = newlen;
      result += chars[divide];
   } while (newlen != 0);

   return result;
}