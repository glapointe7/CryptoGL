
#include "Tools.h"
#include <algorithm>
#include <bitset>
#include <sstream>
#include <iostream>

// Retourne PGCD(a,b).

uint_fast32_t getPGCD(uint_fast32_t a, uint_fast32_t b)
{
   if (b == 0)
      return a;

   return getPGCD(b, a % b);
}

// Retourne le reste d'une division enti�re.

long getIntegerMod(long n, long mod)
{
   long ans = n;
   long x = floor((float) n / mod);
   ans -= (x * mod);
   if (ans < 0)
      ans += mod;

   return ans;
}

// Calcule l'inverse de a modulo n et en retourne la valeur.
// TODO : Vérifier qu'il existe a^{-1} dans Z/nZ.

long getModInverse(long a, const long n)
{
   long i = n, v = 0, d = 1;
   while (a > 0)
   {
      long t = i / a, x = a;
      a = i % x;
      i = x;
      x = d;
      d = v - t * x;
      v = x;
   }
   v %= n;
   if (v < 0) v = (v + n) % n;

   return v;
}

// Retourne la valeur du d�terminant de la matrice initialis�e.

/*long getDeterminant()
{
   return (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);
}*/

// On enl�ve les lettres doublons de la clef.

string removeRepeatedLetters(const string key)
{
   unsigned short keylen = key.length();
   string result(key);

   for (unsigned short i = 0; i < keylen; i++)
   {
      for (unsigned short j = i + 1; j < keylen; j++)
      {
         if (result[i] == result[j])
         {
            result.erase(j, 1);
            keylen--;
         }
      }
   }

   return result;
}

// Remplace dans "text" chaque occurence du caract�re "letter" par "rletter".

void replaceChar(string &text, const char letter, const char rletter)
{
   replace_if(text.begin(), text.end(), [letter](char c) {
      return (c == letter);
   }, rletter);
}

// Effectue, selon 'op', un XOR, NOT, AND, OR et retourne le r�sultat. 

string getBinary(const string &bin_text, const string &bin_to_compare, const unsigned char op)
{
   const size_t size = 500;
   bitset<size> Text(bin_text);
   bitset<size> Comp(bin_to_compare);
   bitset<size> res;

   switch (op)
   {
         // OR
      case '|': res = Text | Comp;
         break;
         // AND
      case '&': res = Text & Comp;
         break;
         // XOR
      case '^': res = Text ^ Comp;
         break;
         // NOT
      case '!': res = Text.flip();
         break;
   }

   return res.to_string<char, char_traits<char>, allocator<char> >();
}

// D�cale les bits � gauche ou � droite selon 'is_right' de 'shift' positions.

string getBinaryShifter(const string &bin_text, const unsigned int shift, bool is_right = true)
{
   const size_t size = 500;
   bitset<size> Text(bin_text);
   bitset<size> res;

   if (is_right)
      res = Text >> shift;
   else
      res = Text << shift;

   return res.to_string<char, char_traits<char>, allocator<char> >();
}

// S�pare une cha�ne de caract�res par l'espace et met chaque mot dans un vecteur.

vector<string> split(const string &text)
{
   istringstream iss(text);
   vector<string> split_text;

   do
   {
      string word;
      iss >> word;
      split_text.push_back(word);
   } while (iss);
   split_text.pop_back();

   return split_text;
}

// Retourne les caract�res pris � chaque 'step' position.

string getStepCharacters(string &text, unsigned short step)
{
   string message = "";
   unsigned long text_len = text.length();

   for (unsigned long i = step; i <= text_len - step; i += step)
   {
      message += text[i];
   }

   return message;
}