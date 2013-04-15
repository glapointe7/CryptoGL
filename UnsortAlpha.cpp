
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
   ClassicalType new_key(removeRepeatedLetters(key));
   ClassicalType str(removeRepeatedLetters(new_key + alpha));
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

const UnsortAlpha::ClassicalType UnsortAlpha::encode(const ClassicalType &clear_text)
{
   ClassicalType crypted = "";
   crypted.reserve(clear_text.length());

   for (auto c : clear_text)
   {
      crypted += unsort_alpha[alpha.find(c)];
   }

   return crypted;
}

// Encode un texte avec un alphabet désordonné choisi.

const UnsortAlpha::ClassicalType UnsortAlpha::decode(const ClassicalType &cipher_text)
{
   ClassicalType decrypted = "";
   decrypted.reserve(cipher_text.length());

   for (auto c : cipher_text)
   {
      decrypted += alpha[unsort_alpha.find(c)];
   }

   return decrypted;
}