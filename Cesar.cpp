
#include "Cesar.h"
#include "Data.h"

Cesar::Cesar()
{
   clear_text = Data::load("clear_text.txt");
   cipher_text = Data::load("cipher_text.txt");
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
std::string Cesar::encode(const std::string &)
{
   string crypted = "";
   crypted.reserve(clear_text.length());

   for (auto c : clear_text)
   {
      crypted += alpha[getAlphaPosition(key, c)];
   }

   Data::save("cipher_text.txt", crypted);

   return crypted;
}

// D�code un texte encod� par le chiffre de Cesar.
string Cesar::decode(const std::string &)
{
   string decrypted = "";
   decrypted.reserve(cipher_text.length());

   for (auto c : cipher_text)
   {
      decrypted += alpha[getAlphaPosition(-key, c)];
   }

   Data::save("clear_text.txt", decrypted);

   return decrypted;
}