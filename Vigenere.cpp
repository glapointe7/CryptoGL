/* 
 * File:   Vigenere.cpp
 * Author: gabriel
 * 
 * Created on March 29, 2013, 8:32 AM
 */

#include "Vigenere.h"
#include "Data.h"

Vigenere::Vigenere()
{
   clear_text = Data::load("clear_text.txt");
   cipher_text = Data::load("cipher_text.txt");
}

Vigenere::Vigenere(const Vigenere& orig)
{
}

Vigenere::~Vigenere()
{
}

void Vigenere::setKey(const string key)
{
   this->key = key;
}

string Vigenere::encode()
{
   string crypted = "";
   crypted.reserve(clear_text.length());

   unsigned int i = 0;
   unsigned int klen = key.length();

   for (auto c : clear_text)
   {
      // Si on atteint la longueur de la clé, alors on recommence au début de la clé.
      if (i == klen)
      {
         i = 0;
      }

      unsigned short pos = (alpha.find(c) + alpha.find(key[i])) % 26;
      crypted += alpha[pos];
      i++;
   }

   return crypted;
}

// D�code un texte encod� par le chiffre de Vigen�re.

string Vigenere::decode()
{
   string decrypted = "";
   decrypted.reserve(cipher_text.length());

   unsigned int i = 0;
   unsigned int klen = key.length();

   for (auto c : cipher_text)
   {
      if (i == klen)
      {
         i = 0;
      }

      unsigned char pos = (alpha.find(c) - alpha.find(key[i]) + 26) % 26;
      decrypted += alpha[pos];
      i++;
   }

   return decrypted;
}