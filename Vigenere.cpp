
#include "Vigenere.h"

using namespace std;

Vigenere::Vigenere()
{
   
}

void Vigenere::setKey(const string &key)
{
   this->key = key;
}

string Vigenere::encode(const std::string &clear_text)
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

string Vigenere::decode(const std::string &cipher_text)
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