
#include "UnsortAlpha.h"

#include "Tools.h"

UnsortAlpha::UnsortAlpha()
{
   unsort_alpha.reserve(alpha.length());
}

void UnsortAlpha::setKey(const std::string key)
{
   this->key = key;
}

// Désordonné un alphabet en concaténant la clé et l'alphabet puis en enlevant les doublons.

void UnsortAlpha::setHorizontalAlpha()
{
   unsort_alpha = removeRepeatedLetters(key + alpha);
}

// Construit l'alphabet désordonné de façon verticale selon 
// la longueur de la clé épurée de doublons.

void UnsortAlpha::setVerticalAlpha()
{
   std::string new_key(removeRepeatedLetters(key));
   std::string str(removeRepeatedLetters(new_key + alpha));
   unsigned int key_len = new_key.length();
   unsigned int alpha_len = str.length();

   for (unsigned int i = 0; i < key_len; i++)
   {
      unsigned int j = i;
      while (j < alpha_len)
      {
         unsort_alpha += str[j];
         j += key_len;
      }
   }
}

// Encode un texte avec un alphabet désordonné choisi.

std::string UnsortAlpha::encode(const std::string &clear_text)
{
   std::string crypted = "";
   crypted.reserve(clear_text.length());

   for (auto c : clear_text)
   {
      crypted += unsort_alpha[alpha.find(c)];
   }

   return crypted;
}

// Encode un texte avec un alphabet désordonné choisi.

std::string UnsortAlpha::decode(const std::string &cipher_text)
{
   std::string decrypted = "";
   decrypted.reserve(cipher_text.length());

   for (auto c : cipher_text)
   {
      decrypted += alpha[unsort_alpha.find(c)];
   }

   return decrypted;
}