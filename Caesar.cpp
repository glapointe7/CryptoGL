
#include "Caesar.hpp"

// Obtient le caractère additionné à key dans l'alphabet.
unsigned char Caesar::getAlphaPosition(const char key, const char c) const
{
   return (alpha.find(c) + key + 26) % 26;
}

void Caesar::setKey(const unsigned char key)
{
   this->key = key;
}

// Encode un texte clair avec le chiffre de Cesar.
const Caesar::ClassicalType Caesar::encode(const ClassicalType &clear_text)
{
   ClassicalType crypted = "";
   crypted.reserve(clear_text.length());

   for (auto c : clear_text)
   {
      crypted += alpha[getAlphaPosition(key, c)];
   }

   return crypted;
}

// D�code un texte encod� par le chiffre de Cesar.
const Caesar::ClassicalType Caesar::decode(const ClassicalType &cipher_text)
{
   ClassicalType decrypted = "";
   decrypted.reserve(cipher_text.length());

   for (auto c : cipher_text)
   {
      decrypted += alpha[getAlphaPosition(-key, c)];
   }

   return decrypted;
}