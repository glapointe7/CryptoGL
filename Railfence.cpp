#include "Railfence.hpp"

#include <string>
#include <utility>

Railfence::Railfence()
{
   setAlpha("ABCDEFGHIJKLMNOPQRSTUVWXYZ.-");
}

void Railfence::setKey(const unsigned int key)
{
   this->key = key;
}

// Encode un texte par le chiffre de RailFence.

const Railfence::ClassicalType Railfence::encode(const ClassicalType &clear_text)
{
   unsigned int clear_len = clear_text.length();
   ClassicalType crypted = "";
   crypted.reserve(clear_len);
   unsigned int n = (key - 1) << 1;

   for (unsigned short i = 0; i < key; i++)
   {
      for (unsigned int j = 0; j < clear_len; j++)
      {
         if (j % n == i || j % n == n - i)
         {
            unsigned short pos = alpha.find(clear_text[j]);
            crypted += alpha[pos];
         }
      }
   }

   return crypted;
}

// Décode un texte encodé par le chiffre de RailFence.

const Railfence::ClassicalType Railfence::decode(const ClassicalType &cipher_text)
{
   unsigned int cipher_len = cipher_text.length();
   ClassicalType decrypted('A', cipher_len);

   auto step = std::make_pair((key - 1) << 1, 0);
   unsigned int k = 0;

   for (unsigned int i = 0; i < key; i++)
   {
      unsigned int j = i;
      decrypted[j] = cipher_text[k];
      k++;
      j += step.first;
      while (j < cipher_len)
      {    
         if(step.first != 0)
         {
            decrypted[j] = cipher_text[k];
            k++;
         }
         j += step.second;
         if (j < cipher_len && step.second != 0)
         {
            decrypted[j] = cipher_text[k];
            k++; 
         }
         j += step.first;
      }
      step.first -= 2;
      step.second += 2;
   }

   return decrypted;
}