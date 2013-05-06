
#include "UnsortAlpha.hpp"

#include "Tools.hpp"
#include "exceptions/EmptyKey.hpp"
#include "exceptions/BadChar.hpp"

UnsortAlpha::UnsortAlpha()
{
   unsort_alpha.reserve(alpha.length());
}

void UnsortAlpha::setKey(const std::string key)
{
   if (key.empty())
   {
      throw EmptyKey("Your key is empty.");
   }

   const char c = badAlphaFound(key);
   if(c != 0)
   {
      throw BadChar("Your key contains at least one character that is not in your alphabet.", c);
   }

   this->key = key;
}

// Désordonné un alphabet en concaténant la clé et l'alphabet puis en enlevant les doublons.

void UnsortAlpha::setHorizontalAlpha()
{
   if (key.empty())
   {
      throw EmptyKey("Your key is not set.");
   }
   
   unsort_alpha = removeRepeatedLetters(key + alpha);
}

// Construit l'alphabet désordonné de façon verticale selon 
// la longueur de la clé épurée de doublons.

void UnsortAlpha::setVerticalAlpha()
{
   if (key.empty())
   {
      throw EmptyKey("Your key is not set.");
   }

   const ClassicalType new_key(removeRepeatedLetters(key));
   const ClassicalType str(removeRepeatedLetters(new_key + alpha));
   const unsigned int key_len = new_key.length();
   const unsigned int alpha_len = str.length();

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

   for (const auto c : clear_text)
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

   for (const auto c : cipher_text)
   {
      decrypted += alpha[unsort_alpha.find(c)];
   }

   return decrypted;
}