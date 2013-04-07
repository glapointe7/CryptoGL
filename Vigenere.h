
#ifndef VIGENERE_H
#define VIGENERE_H

#include "StringCipher.h"

#include <string>

class VigenereBase : public StringCipher
{
public:
   enum class Vigenere_types : uint8_t
   {
      Beaufort = 0,
      BeaufortGerman = 1,
      Rozier = 2,
      Gronsfeld = 3,
      Vixenere = 4,
      Vigenere = 5
   };
   
   virtual ~VigenereBase() {}

   virtual std::string encode(const std::string &) = 0;
   virtual std::string decode(const std::string &) = 0;

   virtual char getChars(const char c, const unsigned int key_pos) const = 0;
   void setKey(const std::string &key);
   
protected:
   std::string get(const std::string &data, const Vigenere_types type);
   std::string getMinusKey() const;
   
   std::string key;
};

// Chiffre de Vigenère : Cipher_text = Key + Clear_text.

class Vigenere : public VigenereBase
{
public:
   std::string encode(const std::string &clear_text)
   {
      return get(clear_text, Vigenere_types::Vigenere);
   }
   
   std::string decode(const std::string &cipher_text)
   {
      setKey(getMinusKey());
      return encode(cipher_text);
   }

   char getChars(const char c, const unsigned int key_pos) const
   {
      return alpha[(alpha.find(key[key_pos]) + alpha.find(c) + 26) % 26];
   }
};

// Chiffre de Beaufort : Cipher_text = Key - Clear_text.

class Beaufort : public VigenereBase
{
public:
   std::string encode(const std::string &clear_text)
   {
      return get(clear_text, Vigenere_types::Beaufort);
   }
   
   std::string decode(const std::string &cipher_text)
   {
      return encode(cipher_text);
   }

   char getChars(const char c, const unsigned int key_pos) const
   {
      return alpha[(alpha.find(key[key_pos]) - alpha.find(c) + 26) % 26];
   }
};

// Chiffre de Beaufort variante allemande : Cipher_text = Clear_text - Key.

class BeaufortGerman : public VigenereBase
{
public:
   std::string encode(const std::string &clear_text)
   {
      return get(clear_text, Vigenere_types::BeaufortGerman);
   }
   
   std::string decode(const std::string &cipher_text)
   {
      setKey(getMinusKey());
      return encode(cipher_text);
   }
   
   char getChars(const char c, const unsigned int key_pos) const
   {
      return alpha[(alpha.find(c) - alpha.find(key[key_pos]) + 26) % 26];
   }
};

// Chiffre de Rozier : Clair + Clef.

/*class Rozier : public VigenereBase
{
public:

   std::string getChars(const char c, const unsigned int key_pos) const
   {
      return std::to_string((alpha.find(c) + key[key_pos]) % 26);
   }
};

// Chiffre de Gronsfeld : Clair + Clef(en chiffres).

class Gronsfeld : public VigenereBase
{
public:

   std::string getChars(const char c, const unsigned int key_pos) const
   {
      return std::to_string((alpha.find(c) + key[key_pos]) % 26);
   }
};

// Chiffre de Vigenere avec multiplication : Clair * Clef.

class Vixenere : public VigenereBase
{
public:

   std::string getChars(const char c, const unsigned int key_pos) const
   {
      return std::to_string(alpha.find(c) * key[key_pos]);
   }
};*/

class VigenereFactory
{
public:

   virtual ~VigenereFactory()
   {
   }

   static VigenereBase* createVigenere(VigenereBase::Vigenere_types V_type)
   {
      switch (V_type)
      {
         case VigenereBase::Vigenere_types::Beaufort:
            return new Beaufort();
         case VigenereBase::Vigenere_types::BeaufortGerman:
            return new BeaufortGerman();
         case VigenereBase::Vigenere_types::Gronsfeld:
           // return new Gronsfeld();
         case VigenereBase::Vigenere_types::Rozier:
           // return new Rozier();
         case VigenereBase::Vigenere_types::Vixenere:
           // return new Vixenere();
         case VigenereBase::Vigenere_types::Vigenere:
            return new Vigenere();
      }
      throw "Vigenere choice is inexistent.";
   }
};

#endif // VIGENERE_H
