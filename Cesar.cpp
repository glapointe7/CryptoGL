
#include "Cesar.h"

Cesar::Cesar()
{
   
}

// Obtient le caractère additionné à key dans l'alphabet.
unsigned char Cesar::getAlphaPosition(const char key, const char c) const
{
   return (alpha.find(c) + key + 26) % 26;
}

void Cesar::setKey(const unsigned char key)
{
   this->key = key;
}

// Encode un texte clair avec le chiffre de Cesar.
std::string Cesar::encode(const std::string &clear_text)
{
   std::string crypted = "";
   crypted.reserve(clear_text.length());

   for (auto c : clear_text)
   {
      crypted += alpha[getAlphaPosition(key, c)];
   }

   return crypted;
}

// D�code un texte encod� par le chiffre de Cesar.
std::string Cesar::decode(const std::string &cipher_text)
{
   std::string decrypted = "";
   decrypted.reserve(cipher_text.length());

   for (auto c : cipher_text)
   {
      decrypted += alpha[getAlphaPosition(-key, c)];
   }

   return decrypted;
}