
#ifndef VIGENERE_HPP
#define VIGENERE_HPP

#include "StringCipher.hpp"

#include <functional>
#include <string>
#include <sstream>

#include "Tools.hpp"  // fonction split

#include "exceptions/BadChar.hpp"
#include "exceptions/EmptyKey.hpp"

// Vigenere : CIPHER = CLEAR + KEY

class Vigenere : public StringCipher
{
protected:
   typedef std::function<ClassicalType(const ClassicalType &, const char, const char)> GetCharFunction;
   
   Vigenere(const GetCharFunction &charEncode, const GetCharFunction &charDecode)
   : charEncode(charEncode), charDecode(charDecode) {}

   static const ClassicalType clearPlusKey(const ClassicalType &alpha, const char c, const char key_pos)
   {
      return ClassicalType(1, alpha[(alpha.find(c) + alpha.find(key_pos)) % alpha.length()]);
   }

   static const ClassicalType clearMinusKey(const ClassicalType &alpha, const char c, const char key_pos)
   {
      return ClassicalType(1, alpha[(alpha.find(c) - alpha.find(key_pos) + alpha.length()) % alpha.length()]);
   }

   static const ClassicalType keyMinusClear(const ClassicalType &alpha, const char c, const char key_pos)
   {
      return ClassicalType(1, alpha[(alpha.find(key_pos) - alpha.find(c) + alpha.length()) % alpha.length()]);
   }

   const GetCharFunction charEncode, charDecode;
   std::string key;

public:
   Vigenere()
   : charEncode(clearPlusKey), charDecode(clearMinusKey) {}

   const ClassicalType encode(const ClassicalType &clear_text);
   const ClassicalType decode(const ClassicalType &cipher_text);
   
   void setKey(const std::string &key);

private:   
   const ClassicalType process(const ClassicalType &text, const GetCharFunction &getNextChar);
   
   
};

// Beaufort : CIPHER = -CLEAR + KEY

class Beaufort : public Vigenere
{
public:

   Beaufort()
   : Vigenere(keyMinusClear, keyMinusClear) {}
};

// Beaufort German : CIPHER = CLEAR - KEY

class BeaufortGerman : public Vigenere
{
public:

   BeaufortGerman()
   : Vigenere(clearMinusKey, clearPlusKey) {}
};

// Rozier : CIPHER = CLEAR + KEY

class Rozier : public Vigenere
{
public:

   Rozier()
   : Vigenere(clearPlusKey, clearMinusKey) {}

   void setKey(const ClassicalType &rozier_key)
   {
      const uint32_t key_length = rozier_key.length();
      const uint8_t alpha_len = alpha.length();

      for (uint32_t i = 0; i < key_length - 1; ++i)
      {
         key += alpha[(alpha.find(rozier_key[i + 1]) - alpha.find(rozier_key[i]) + alpha_len) % alpha_len];
      }
      key += alpha[(alpha.find(rozier_key[key_length - 1]) - alpha.find(rozier_key[0]) + alpha_len) % alpha_len];
   }
};

// Chiffre de Caesar : Cipher = Clear + key où key est un entier char.

class Caesar : public Vigenere
{
public:

   Caesar()
   : Vigenere(clearPlusKey, clearMinusKey) {}

   // Si la clé est négative et > alpha_len, on doit la remettre dans {0,...,alpha_len-1}.

   void setKey(const char caesar_key)
   {
      const int8_t alpha_len = alpha.length();
      char the_key = caesar_key % alpha_len;
      the_key = (the_key + alpha_len) % alpha_len;
      key = std::string(1, alpha[the_key]);
   }
};

// VigenereMult : CIPHER = CLEAR * KEY

class VigenereMult : public Vigenere
{

   static const ClassicalType clearMultKey(const ClassicalType &alpha, const char c, const char key_pos)
   {
      std::stringstream ss;
      ss << ((alpha.find(c) + 1) * (alpha.find(key_pos) + 1)) << " ";
      return ss.str();
   }

   static const ClassicalType keyDivideCipher(const ClassicalType &alpha, const unsigned short c, const char key_pos)
   {
      return ClassicalType(1, alpha[(c / (alpha.find(key_pos) + 1)) - 1]);
   }

public:

   VigenereMult()
   : Vigenere(clearMultKey, keyDivideCipher) {}

   const ClassicalType decode(const ClassicalType &cipher_text)
   {
      const uint32_t key_length = key.length();

      ClassicalType toReturn = "";
      std::vector<std::string> cipher_numbers(split(cipher_text));
      toReturn.reserve(cipher_text.length());

      uint32_t idx = 0;
      for (const auto number : cipher_numbers)
      {
         uint16_t num;
         std::istringstream(number) >> num;
         toReturn += keyDivideCipher(alpha, num, key[idx]);
         idx = (idx + 1) % key_length;
      }

      return toReturn;
   }
};

class Gronsfeld : public Vigenere
{
public:
   Gronsfeld()
   : Vigenere(clearPlusKey, clearMinusKey) {}
   
   void setKey(const std::vector<int32_t> &grons_key)
   {
      const int8_t alpha_len = alpha.length();
      key.clear();
      key.reserve(grons_key.size());
      
      for(const auto number : grons_key)
      {
         const int8_t x = number % alpha_len;
         key += alpha[(x + alpha_len) % alpha_len];
      }
   }
};

#endif