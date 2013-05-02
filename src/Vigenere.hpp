
#ifndef VIGENERE_HPP
#define VIGENERE_HPP

#include "StringCipher.hpp"

#include <functional>
#include <string>
#include <sstream>

#include "Tools.hpp"  // fonction split
#include "exceptions/EmptyKey.hpp"
#include "exceptions/BadChar.hpp"

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

   ClassicalType key;
   const GetCharFunction charEncode, charDecode;

public:
   typedef ClassicalType Key;

   Vigenere()
   : charEncode(clearPlusKey), charDecode(clearMinusKey) {}

   void setKey(const Key &key)
   {
      if(key.empty())
      {
         throw EmptyKey("Your key is empty.");
      }
      
      if(badAlphaFound(key))
      {
         throw BadChar("Your key contains at least one character that is not in your alphabet.");
      }
      
      this->key = key;
   }

   const ClassicalType encode(const ClassicalType &clear_text)
   {
      if(key.empty())
      {
         throw EmptyKey("Your key is not set.");
      }
      
      return process(clear_text, charEncode);
   }

   const ClassicalType decode(const ClassicalType &cipher_text)
   {
      if(key.empty())
      {
         throw EmptyKey("Your key is not set.");
      }
      
      return process(cipher_text, charDecode);
   }

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

   virtual void setKey(const ClassicalType &v_key) final
   {
      if(v_key.empty())
      {
         throw EmptyKey("Your key is empty.");
      }
      
      if(badAlphaFound(v_key))
      {
         throw BadChar("Your key contains at least one character that is not in your alphabet.");
      }
      
      const unsigned int key_length = v_key.length();
      const uint32_t alpha_len = alpha.length();
      key = "";
      key.reserve(key_length);

      for (unsigned int i = 0; i < key_length - 1; ++i)
      {
         key += alpha[(alpha.find(v_key[i + 1]) - alpha.find(v_key[i]) + alpha_len) % alpha_len];
      }
   }
};

// Chiffre de Caesar : Cipher = Clear + key où key est un entier char.

class Caesar : public Vigenere
{
public:

   Caesar()
   : Vigenere(clearPlusKey, clearMinusKey) {}

   // Si la clé est négative et > alpha_len, on doit la remettre dans {0,...,alpha_len-1}.
   void setKey(const char c_key)
   {      
      const int16_t alpha_len = alpha.length();
      char the_key = c_key % alpha_len;
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
   : Vigenere(clearMultKey, keyDivideCipher)
   {}

   const ClassicalType decode(const ClassicalType &cipher_text)
   {
      if(key.empty())
      {
         throw EmptyKey("Your key is not set.");
      }
      
      const unsigned int key_length = key.length();

      ClassicalType toReturn = "";
      std::vector<std::string> cipher_numbers(split(cipher_text));
      toReturn.reserve(cipher_text.length());

      unsigned int idx = 0;
      for (auto number : cipher_numbers)
      {
         unsigned short num;
         std::istringstream(number) >> num;
         toReturn += keyDivideCipher(alpha, num, key[idx]);
         idx = (idx + 1) % key_length;
      }

      return toReturn;
   }
};

#endif