
#include "Vigenere.h"

void VigenereBase::setKey(const std::string &key)
{
   this->key = key;
}

std::string VigenereBase::get(const std::string &data, const Vigenere_types type)
{
   std::string text = "";
   text.reserve(data.length());

   unsigned int i = 0;
   unsigned int klen = key.length();

   VigenereBase *V = VigenereFactory::createVigenere(type);
   
   for (auto c : data)
   {
      // Si on atteint la longueur de la clé, alors on recommence au début de la clé.
      if (i == klen)
      {
         i = 0;
      }
      
       text += V->getChars(c, i);
      i++;
   }
   
   return text;
}

// Retourne la clé inversée dans l'alphabet donné.
std::string VigenereBase::getMinusKey() const
{
   std::string minus_key = "";
   unsigned int key_len = key.length();
   minus_key.reserve(key_len);
   
   for(auto c : key)
   {
      minus_key += alpha[26 - alpha.find(c) - 1];
   }
   
   return minus_key;
}

/*std::string Vigenere::encode(const std::string &clear_text)
{
   std::string crypted = "";
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
      
      crypted += alpha[(alpha.find(c) + alpha.find(key[i])) % 26];
      i++;
   }

   return crypted;
}

// D�code un texte encod� par le chiffre de Vigen�re.

std::string Vigenere::decode(const std::string &cipher_text)
{
   std::string decrypted = "";
   decrypted.reserve(cipher_text.length());

   unsigned int i = 0;
   unsigned int klen = key.length();

   for (auto c : cipher_text)
   {
      if (i == klen)
      {
         i = 0;
      }

      decrypted += alpha[(alpha.find(c) - alpha.find(key[i]) + 26) % 26];
      i++;
   }

   return decrypted;
}*/