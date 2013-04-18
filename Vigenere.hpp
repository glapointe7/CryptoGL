
#ifndef VIGENERE_HPP
#define VIGENERE_HPP

#include "StringCipher.hpp"

#include <functional>
#include <string>
#include <sstream>

#include "Tools.hpp"  // fonction split

// Vigenere : CIPHER = CLEAR + KEY

class Vigenere : public StringCipher
{
protected:
   typedef std::function<ClassicalType(const ClassicalType &, const char, const char)> GetCharFunction;

   Vigenere(const GetCharFunction &charEncode, const GetCharFunction &charDecode)
   : charEncode(charEncode), charDecode(charDecode) {}

   static const ClassicalType clearPlusKey(const ClassicalType &alpha, const char c, const char key_pos)
   {
      return ClassicalType(1, alpha[(alpha.find(c) + alpha.find(key_pos)) % 26]);
   }

   static const ClassicalType clearMinusKey(const ClassicalType &alpha, const char c, const char key_pos)
   {
      return ClassicalType(1, alpha[(alpha.find(c) - alpha.find(key_pos) + 26) % 26]);
   }

   static const ClassicalType keyMinusClear(const ClassicalType &alpha, const char c, const char key_pos)
   {
      return ClassicalType(1, alpha[(alpha.find(key_pos) - alpha.find(c) + 26) % 26]);
   }

   ClassicalType key;
   const GetCharFunction charDecode, charEncode;

public:
   typedef ClassicalType Key;

   Vigenere()
   : charEncode(clearPlusKey), charDecode(clearMinusKey) {}

   virtual void setKey(const Key &key)
   {
      this->key = key;
   }

   const ClassicalType encode(const ClassicalType &clear_text)
   {
      return process(clear_text, charEncode);
   }

   const ClassicalType decode(const ClassicalType &cipher_text)
   {
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

   void setKey(const ClassicalType &v_key)
   {
      const unsigned int key_length = v_key.length();
      key = "";
      key.reserve(key_length);

      for (unsigned int i = 0; i < key_length - 1; ++i)
      {
         key += alpha[(alpha.find(v_key[i + 1]) - alpha.find(v_key[i]) + 26) % 26];
      }
   }
};

// Chiffre de Caesar : Cipher = Clear + key où key est un entier char.

class Caesar : public Vigenere
{
public:

   Caesar()
   : Vigenere(clearPlusKey, clearMinusKey) {}

   void setKey(const unsigned char c_key)
   {
      key = std::string(1, alpha[c_key]);
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
   {
   }

   const ClassicalType decode(const ClassicalType &cipher_text)
   {
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


/*class VigenereFactory
{
public:
   enum Type { Beaufort = 0, BeaufortGerman, Rozier, VigenereMult, Vigenere};

   static Vigenere* createVigenere(const Type type)
   {
      switch (type)
      {	 
         case Vigenere::Type::Beaufort:
            return new Vigenere(VigenereFunctions::keyMinusClear, VigenereFunctions::keyMinusClear);
         case Vigenere::Type::BeaufortGerman:
            return new Vigenere(VigenereFunctions::clearMinusKey, VigenereFunctions::clearPlusKey);
         case Vigenere::Type::Rozier:
            return new Rozier(VigenereFunctions::clearPlusKey, VigenereFunctions::clearMinusKey);
            //return new Vigenere(VigenereFunctions::clearPlusKey, VigenereFunctions::clearMinusKey);
         case Vigenere::Type::VigenereMult:
          //return new VigenereMult(VigenereFunctions::clearMultKey, VigenereFunctions::keyDivideCipher);
         case Vigenere::Type::Vigenere:
            return new Vigenere(VigenereFunctions::clearPlusKey, VigenereFunctions::clearMinusKey);
      }
      throw "Vigenere choice is inexistent.";
   }
};*/

#endif