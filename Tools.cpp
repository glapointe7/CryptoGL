
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

bool isSuperIncresing(const std::vector<unsigned long> &sequence)
{
   unsigned long sum = 0;

   for (auto number : sequence)
   {
      if (number > sum)
      {
         sum += number;
      }
      else
      {
         return false;
      }
   }

   return true;
}

// On enl�ve les lettres doublons de la clef.

std::string removeRepeatedLetters(const std::string key)
{
   unsigned short keylen = key.length();
   std::string result(key);

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

void replaceChar(std::string &text, const char letter, const char rletter)
{
   std::replace_if(text.begin(), text.end(), [letter](char c) {
      return (c == letter);
   }, rletter);
}

// Effectue, selon 'op', un XOR, NOT, AND, OR et retourne le r�sultat. 

std::string getBinary(const std::string &bin_text, const std::string &bin_to_compare, const unsigned char op)
{
   const size_t size = 500;
   std::bitset<size> Text(bin_text);
   std::bitset<size> Comp(bin_to_compare);
   std::bitset<size> res;

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

   return res.to_string();
}

// D�cale les bits � gauche ou � droite selon 'is_right' de 'shift' positions.

std::string getBinaryShifter(const std::string &bin_text, const unsigned int shift, bool is_right = true)
{
   const size_t size = 500;
   std::bitset<size> Text(bin_text);
   std::bitset<size> res;

   if (is_right)
      res = Text >> shift;
   else
      res = Text << shift;

   return res.to_string();
}

// S�pare une cha�ne de caract�res par l'espace et met chaque mot dans un vecteur.

std::vector<std::string> split(const std::string &text)
{
   std::istringstream iss(text);
   std::vector<std::string> split_text;

   do
   {
      std::string word;
      iss >> word;
      split_text.push_back(word);
   } while (iss);
   split_text.pop_back();

   return split_text;
}

std::vector<std::vector<bool> > getBlockBinary(const std::vector<bool> &bytes, const unsigned long block_size)
{
   std::vector<std::vector<bool> > blocks_bits;
   unsigned long bytes_size = bytes.size();
   
   
   for(unsigned long i = 0; i < bytes_size; i += block_size)
   {
      std::vector<bool> block(bytes.begin() + i, bytes.begin() + i + block_size);
      blocks_bits.push_back(block);
   }
   
   return blocks_bits;
}

// Retourne les caract�res pris � chaque 'step' position.

std::string getStepCharacters(std::string &text, unsigned short step)
{
   std::string message = "";
   unsigned long text_len = text.length();

   for (unsigned long i = step; i <= text_len - step; i += step)
   {
      message += text[i];
   }

   return message;
}

std::vector<unsigned char> getBytes(const std::vector<bool> &bits)
{
   unsigned long bits_size = bits.size();
   std::vector<unsigned char> bytes;
   
   for(unsigned long i = 0; i < bits_size; i += 8)
   {
      unsigned char k = 0;
      unsigned char ascii = 0;
      for(char j = 7; j >= 0; --j)
      {
         ascii += bits[i+k] << j;
         k++;
      }
      bytes.push_back(ascii);
   }
   
   return bytes;
}