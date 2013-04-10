
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

class Vigenere : public StringCipher
{
   typedef std::function<unsigned int(const char c, const unsigned int)> GetCharFunction;

public:
   // Ajouter Cesar aussi ???
   enum class Type : uint8_t
   {
      Beaufort = 0, BeaufortGerman, Rozier, Vixenere, Vigenere
   };

   // TODO Placer ici les différentes actions (que tu as expliqués en commentaire en haut)
   unsigned int encode1(const char c, const unsigned int key_pos) { return 1; }
   unsigned int encode2(const char c, const unsigned int key_pos) { return 1; }
   unsigned int decode1(const char c, const unsigned int key_pos) { return 1; }
   unsigned int decode2(const char c, const unsigned int key_pos) { return 1; }

   Vigenere(GetCharFunction charDecode, GetCharFunction charEncode);

   void setKey(const std::string &key);

   virtual std::string encode(const std::string &);
   virtual std::string decode(const std::string &);

   virtual char getChars(const char c, const unsigned int key_pos) const;
   
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
         // TODO Envoyer les bonnes fonctions en paramètre	 
         case Vigenere::Type::Beaufort:
            return new Vigenere(encode1, decode1);
         case Vigenere::Type::BeaufortGerman:
            return new Vigenere(encode1, decode2);
         case Vigenere::Type::Rozier:
            return new Vigenere(encode2, decode1);
         case Vigenere::Type::Vixenere:
          //return new Vigenere(encode1, decode1);
         case Vigenere::Type::Vigenere:
            return new Vigenere(encode2, decode2);
      }
      throw "Vigenere choice is inexistent.";
   }
};

#endif // VIGENERE_H

