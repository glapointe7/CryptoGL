#include "converterTools.h"
#include <bitset>
#include <sstream>
#include <algorithm>
#include <vector>

// Convertit un string en une suite binaire.

string convertTextToBinaryString(const string &text)
{
   string bits = "";

   for (auto c : text)
   {
      bitset<8> byte(c);
      bits += byte.to_string();
   }

   return bits;
}

// Convertit un texte en majuscules.

void convertTextToMajus(string &text)
{
   transform(text.begin(), text.end(), text.begin(), ::toupper);
}

// Transforme un String en un nombre.

template <class Type>
Type lexical_cast(const string &textNumber)
{
   istringstream ss(textNumber);

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
string baseConvertString(const string number, const unsigned char from_base,
        const unsigned char to_base)
{
   string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_";
   string result = "";
   vector<unsigned char> pos;

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