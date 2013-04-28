
#include "Tools.hpp"

#include <algorithm>
#include <bitset>
#include <sstream>

// Retourne PGCD(a,b).

uint_fast32_t getPGCD(uint_fast32_t a, uint_fast32_t b)
{
   if (b == 0)
      return a;

   return getPGCD(b, a % b);
}

// Retourne le reste d'une division enti�re.

long getIntegerMod(const long n, const long mod)
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

uint64_t getLegendreSymbol(const uint64_t x, const uint64_t e, const uint64_t n)
{
   uint64_t a = 1;
   unsigned char e_size = sizeof (e);
   for (char i = e_size; i >= 0; --i)
   {
      a = (a * a) % n;
      if (getBitAtPosition(i, e) != 0)
      {
         a = (a * x) % n;
      }
   }

   return a;
}

// On enl�ve les lettres doublons de la clef.

std::string removeRepeatedLetters(const std::string key)
{
   unsigned int keylen = key.length();
   std::string result(key);

   for (unsigned int i = 0; i < keylen; i++)
   {
      for (unsigned int j = i + 1; j < keylen; j++)
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
   std::vector < std::vector<bool> > blocks_bits;
   unsigned long bytes_size = bytes.size();


   for (unsigned long i = 0; i < bytes_size; i += block_size)
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

   for (unsigned long i = 0; i < bits_size; i += 8)
   {
      unsigned char k = 0;
      unsigned char ascii = 0;
      for (char j = 7; j >= 0; --j)
      {
         ascii += bits[i + k] << j;
         k++;
      }
      bytes.push_back(ascii);
   }

   return bytes;
}

void eraseChars(std::string &text, const std::string chars)
{
   for (auto c : chars)
   {
      text.erase(std::remove(text.begin(), text.end(), c), text.end());
   }
}

// Retourne le bit à la position pos de number.

const bool getBitAtPosition(const unsigned long pos, const uint64_t &number)
{
   const uint64_t x = 1;
   return (number & (x << pos)) > 0;
}

uint32_t getBitsRange(const uint32_t number, const unsigned int from, const unsigned int to)
{
   uint32_t mask = 1;
   uint32_t result;
  
   for (uint32_t i = from; i < to; ++i)
   {
      if (getBitAtPosition(i, number) == 1)
      {
         result |= mask;
      }
      mask <<= 1;
   }

   return result;
}

// Rotation circulaire à gauche des bits de pos fois.

uint32_t rotl32(const uint32_t number, const unsigned char pos)
{
   return (number << pos) | (number >> (32 - pos));
}

// Rotation circulaire à droite des bits de pos fois.

uint32_t rotr32(const uint32_t number, const unsigned char pos)
{
   return (number >> pos) | (number << (32 - pos));
}

const uint64_t rotateRight(const int64_t &value, const unsigned char shift, const unsigned char max)
{
   uint64_t x = 1;
   return ((value >> shift) | (value << (max - shift))) & ((x << max) - 1);
}

// bits doit être <= 64. 

const uint64_t rotateLeft(const int64_t &value, const unsigned char shift, const unsigned char max)
{
   uint64_t x = 1;
   return ((value << shift) | (value >> (max - shift))) & ((x << max) - 1);
}

void endianSwap32(uint32_t &value)
{
   value = (value >> 24) | ((value << 8) & 0x00FF0000) |
           ((value >> 8) & 0x0000FF00) | (value << 24);
}

void endianSwap64(uint64_t &value)
{
   value = (value >> 56) |
           ((value << 40) & 0x00FF000000000000uLL) |
           ((value << 24) & 0x0000FF0000000000uLL) |
           ((value << 8) & 0x000000FF00000000uLL) |
           ((value >> 8) & 0x00000000FF000000uLL) |
           ((value >> 24) & 0x0000000000FF0000uLL) |
           ((value >> 40) & 0x000000000000FF00uLL) |
           (value << 56);
}

// NOTE : Le premier bit correspond au MSB de data, donc 2^63 pour un 64 bits.

const uint64_t getBitsFromTable(const uint64_t &data, const Table &table, const uint64_t from, const uint64_t to)
{
   const uint64_t y = 1;
   unsigned char i = to;
   uint64_t output = 0;
   for (auto row : table)
   {
      for (auto byte : row)
      {
         --i;
         // Si le bit à la position byte MSB est 1, alors on le positionne selon i MSB.
         if ((data >> (from - byte)) & 0x1)
         {
            output |= y << i;
         }
      }
   }

   return output;
}