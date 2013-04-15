
#ifndef VIGENERE_H
#define VIGENERE_H

#include "StringCipher.h"

#include <functional>
#include <string>

// Vigenere : CIPHER = CLEAR + KEY
// Beaufort : CIPHER = -CLEAR + KEY
// Beaufort German : CIPHER = CLEAR - KEY
// Vixenere : CIPHER = CLEAR * KEY
// Rozier : CIPHER = CLEAR + KEY (en modifiant la clé selon l'algo).

class Vigenere : public StringCipher
{
protected:
   typedef std::function<String(const String &, const char, const unsigned int)> GetCharFunction;
   
   Vigenere(const GetCharFunction &charEncode, const GetCharFunction &charDecode)
      : charDecode(charDecode), charEncode(charEncode) {}

public:
   typedef String Key;

   Vigenere()
      : charDecode(clearPlusKey), charEncode(clearMinusKey) {}

   virtual void setKey(const Key &key) { this->key = key; }

   const String encode(const String &clear_text)
   {
      return process(clear_text, charEncode);
   }

   const String decode(const String &cipher_text)
   {
      return process(cipher_text, charDecode);
   }

protected:

   static const String clearPlusKey(const String &alpha, const char c, const unsigned int key_pos) 
   { 
      // TODO All these functions
      return ""; // std::to_string((c + key_pos) % 26);
   }

   static const String clearMinusKey(const String &alpha, const char c, const unsigned int key_pos) 
   { 
      return ""; // std::to_string((c - key_pos + 26) % 26);
   }

   static const String keyMinusClear(const String &alpha, const char c, const unsigned int key_pos) 
   { 
      return ""; // std::to_string((key_pos - c + 26) % 26);
   }

   String key;
   const GetCharFunction &charDecode, &charEncode;

private:
   const String process(const String &text, const GetCharFunction &getNextChar)
   {
      const unsigned int key_length = key.length();
   
      String toReturn = "";
      toReturn.reserve(text.length());

      unsigned int idx = 0;
      for (auto c : text)
      {
         toReturn += getNextChar(alpha, c, idx);
         idx = (idx + 1) % key_length;
      }
   
      return toReturn;
   }
};

class Beaufort : public Vigenere
{
public:
   Beaufort()
      : Vigenere(keyMinusClear, keyMinusClear) {}
};

class BeaufortGerman : public Vigenere
{
public:
   BeaufortGerman()
      : Vigenere(clearMinusKey, clearPlusKey) {}
};

class Rozier : public Vigenere
{
public:
   Rozier()
      : Vigenere(keyMinusClear, keyMinusClear) {}

   void setKey(const String &v_key)
   {
      const unsigned int key_length = v_key.length();
      key = "";
      key.reserve(key_length);
      
      for (unsigned int i = 0; i < key_length - 1; ++i)
      {
         key += alpha[(alpha.find(v_key[i+1]) - alpha.find(v_key[i]) + 26) % 26];
      }
   }
};

class VigenereMult : public Vigenere
{
   static const String clearMultKey(const String &alpha, const char c, const unsigned int key_pos) 
   {
      // TODO Return number as you wanted to do, ie "42" and other :)
      // Ca retourne un string JUSTE pour ce cas-ci en plus
      return ""; //std::to_string(c * key_pos);
   }

   static const String keyDivideCipher(const String &alpha, const unsigned int c, const unsigned int key_pos) 
   {
      // TODO Idem
      return ""; //std::to_string(c / key_pos);
   }

public:
   VigenereMult()
      : Vigenere(clearMultKey, keyDivideCipher) {}

   // TODO encode() should be ok Gab

   const String decode(const String &cipher_text)
   {
      return ""; // Must override at least decode because it must split string
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

#endif // VIGENERE_H

