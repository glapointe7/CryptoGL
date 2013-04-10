
#ifndef VIGENERE_H
#define VIGENERE_H

#include <functional>

#include "StringCipher.h"

#include <string>

// Vigenere : CIPHER = CLEAR + KEY
// Beaufort : CIPHER = -CLEAR + KEY
// Beaufort German : CIPHER = CLEAR - KEY
// Vixenere : CIPHER = CLEAR * KEY
// Rozier : CIPHER = CLEAR + KEY (en modifiant la clé selon l'algo).

// Ajouter Cesar qui est un Vigenere avec une clé de longueur 1.

namespace VigenereFunctions
{
   // TODO Placer ici les différentes actions (que tu as expliqués en commentaire en haut)
   unsigned char clearPlusKey(const char c, const char key_pos) 
   { 
      return (c + key_pos) % 26; 
   }
   
   unsigned char clearMinusKey(const char c, const char key_pos) 
   { 
      return (c - key_pos + 26) % 26; 
   }
   
   unsigned char keyMinusClear(const char c, const char key_pos) 
   { 
      return (key_pos - c + 26) % 26; 
   }
}

class Vigenere : public StringCipher
{
   typedef std::function<unsigned char(const char c, const char k)> GetCharFunction;

public:
   
   enum class Type : uint8_t
   {
      Beaufort = 0, BeaufortGerman, Rozier, Vixenere, Vigenere
   };

   Vigenere(GetCharFunction charDecode, GetCharFunction charEncode);

   void setKey(const std::string &key);

   virtual std::string encode(const std::string &);
   virtual std::string decode(const std::string &);
   
private:
   std::string key;
   GetCharFunction charDecode, charEncode;
};

class VigenereFactory
{
public:
   static Vigenere* createVigenere(const Vigenere::Type type)
   {
      switch (type)
      {	 
         case Vigenere::Type::Beaufort:
            return new Vigenere(VigenereFunctions::keyMinusClear, VigenereFunctions::keyMinusClear);
         case Vigenere::Type::BeaufortGerman:
            return new Vigenere(VigenereFunctions::clearMinusKey, VigenereFunctions::clearPlusKey);
         case Vigenere::Type::Rozier:
            return new Vigenere(VigenereFunctions::clearPlusKey, VigenereFunctions::clearMinusKey);
         case Vigenere::Type::Vixenere:
          //return new Vigenere(encode1, decode1);
         case Vigenere::Type::Vigenere:
            return new Vigenere(VigenereFunctions::clearPlusKey, VigenereFunctions::clearMinusKey);
      }
      throw "Vigenere choice is inexistent.";
   }
};

#endif // VIGENERE_H

