
#ifndef VIGENERE_HPP
#define VIGENERE_HPP

#include "StringCipherWithStringKey.hpp"

#include <functional>
#include <string>
#include <vector>

#include "Tools.hpp"  // fonction split
#include "String.hpp"
#include "Types.hpp"

// Vigenere : CIPHER = CLEAR + KEY

class Vigenere : public StringCipherWithStringKey
{
private:
   using GetCharFunction = std::function<const ClassicalType(const ClassicalType &, const char, const char)>;
   const GetCharFunction charEncode, charDecode;
   const ClassicalType process(const ClassicalType &text, const GetCharFunction &getNextChar);
   
protected:   
   Vigenere(const GetCharFunction &charEncode, const GetCharFunction &charDecode, const KeyType &key)
      : charEncode(charEncode), charDecode(charDecode) { setKey(key); }
   
   Vigenere(const GetCharFunction &charEncode, const GetCharFunction &charDecode)
      : charEncode(charEncode), charDecode(charDecode) {}

   static const ClassicalType clearPlusKey(const ClassicalType &alpha, const char c, const char key_pos)
   {
      const uint32_t x = (alpha.find(c) + alpha.find(key_pos)) % alpha.length();
      return ClassicalType(1, alpha[x]);
   }

   static const ClassicalType clearMinusKey(const ClassicalType &alpha, const char c, const char key_pos)
   {
      const uint32_t x = (alpha.find(c) - alpha.find(key_pos) + alpha.length()) % alpha.length();
      return ClassicalType(1, alpha[x]);
   }

   static const ClassicalType keyMinusClear(const ClassicalType &alpha, const char c, const char key_pos)
   {
      return ClassicalType(1, alpha[(alpha.find(key_pos) - alpha.find(c) + alpha.length()) % alpha.length()]);
   }

public:
   explicit Vigenere(const KeyType &key)
      : Vigenere(clearPlusKey, clearMinusKey, key) {}
   virtual ~Vigenere() {}

   virtual const ClassicalType encode(const ClassicalType &clear_text) final;
   virtual const ClassicalType decode(const ClassicalType &cipher_text);
};

// Beaufort : CIPHER = -CLEAR + KEY

class Beaufort : public Vigenere
{
public:
   explicit Beaufort(const KeyType &key)
      : Vigenere(keyMinusClear, keyMinusClear, key) {}
};

// Beaufort German : CIPHER = CLEAR - KEY

class BeaufortGerman : public Vigenere
{
public:
   explicit BeaufortGerman(const KeyType &key)
      : Vigenere(clearMinusKey, clearPlusKey, key) {}
};

// Rozier : CIPHER = CLEAR + KEY

class Rozier : public Vigenere
{
public:
   explicit Rozier(const KeyType &key)
      : Vigenere(clearPlusKey, clearMinusKey) { setKey(key); }

   /* Specific to Rozier cipher : the rozier_key is a string. */
   void setKey(const ClassicalType &rozier_key)
   {
      const uint32_t key_length = rozier_key.length();
      const uint8_t alpha_len = alpha.length();
      KeyType new_key;
      new_key.reserve(key_length);

      for (uint32_t i = 0; i < key_length - 1; ++i)
      {
         new_key += alpha[(alpha.find(rozier_key[i + 1]) - alpha.find(rozier_key[i]) + alpha_len) % alpha_len];
      }
      new_key += alpha[(alpha.find(rozier_key[key_length - 1]) - alpha.find(rozier_key[0]) + alpha_len) % alpha_len];
      
      StringCipherWithStringKey::setKey(new_key);
   }
};

// Caesar : Cipher = Clear + key where key is a byte.

class Caesar : public Vigenere
{   
public:
   using StringCipherWithStringKey::setKey;

   explicit Caesar(const uint8_t key)
      : Vigenere(clearPlusKey, clearMinusKey) { setKey(key); }

   // If the key is < 0 and > alpha_len, we make sure the key will be in the set {0,...,alpha_len-1}.
   /* Specific to Caesar cipher : the caesar_key is a byte. */
   void setKey(const int8_t caesar_key)
   {
      const uint8_t alpha_len = alpha.length();
      int8_t the_key = caesar_key % alpha_len;
      the_key = (the_key + alpha_len) % alpha_len;
      const KeyType new_key = KeyType(1, alpha[the_key]);
      StringCipherWithStringKey::setKey(new_key);
   }
};

// VigenereMult : CIPHER = CLEAR * KEY

class VigenereMult : public Vigenere
{
private:
   static const ClassicalType clearMultKey(const ClassicalType &alpha, const uint8_t c, const uint8_t key_pos)
   {
      const uint32_t x = (alpha.find(c) + 1) * (alpha.find(key_pos) + 1);
      ClassicalType buffer = String::uintToString(x);
      buffer.reserve(buffer.length() + 1);
      buffer.push_back(' '); 
      
      return buffer;
   }

   static const ClassicalType keyDivideCipher(const ClassicalType &alpha, const uint16_t c, const uint8_t key_pos)
   {
      const uint8_t x = (c / (alpha.find(key_pos) + 1)) - 1;
      return ClassicalType(1, alpha[x]);
   }

public:
   explicit VigenereMult(const KeyType &key)
      : Vigenere(clearMultKey, keyDivideCipher, key) {}

   /* Decode the Vigenere Multiplication cipher with the given cipher_text. */
   virtual const ClassicalType decode(const ClassicalType &cipher_text) final
   {
      const KeyType my_key = getKey();
      const uint32_t key_length = my_key.length();

      ClassicalType toReturn;
      const std::vector<ClassicalType> cipher_numbers = Tools::split(cipher_text);
      toReturn.reserve(cipher_text.length());

      uint32_t idx = 0;
      for (const auto number : cipher_numbers)
      {
         toReturn.append(keyDivideCipher(alpha, atoi(number.c_str()), my_key[idx]));
         idx = (idx + 1) % key_length;
      }

      return toReturn;
   }
};

class Gronsfeld : public Vigenere
{  
public:
   using StringCipherWithStringKey::setKey;
   
   explicit Gronsfeld(const Int32Vector &key)
      : Vigenere(clearPlusKey, clearMinusKey) { setKey(key); }
   
   /* Specific to Gronsfeld cipher : the grons_key is a vector of integers. */
   void setKey(const Int32Vector &grons_key)
   {
      const uint8_t alpha_len = alpha.length();
      KeyType new_key;
      new_key.reserve(grons_key.size());
      
      for(const auto number : grons_key)
      {
         const uint8_t x = ((number % alpha_len) + alpha_len) % alpha_len;
         new_key.push_back(alpha[x]);
      }
      StringCipherWithStringKey::setKey(new_key);
   }
};

#endif