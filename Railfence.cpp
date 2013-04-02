#include "Railfence.h"

#include <string>
#include <utility>

using namespace std;

Railfence::Railfence()
{
   setAlpha("ABCDEFGHIJKLMNOPQRSTUVWXYZ.-");
}

void Railfence::setKey(const unsigned int key)
{
   this->key = key;
}

// Encode un texte par le chiffre de RailFence.

string Railfence::encode(const std::string &clear_text)
{
   unsigned int clear_len = clear_text.length();
   string crypted = "";
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

string Railfence::decode(const string &cipher_text)
{
   unsigned int cipher_len = cipher_text.length();
   string decrypted('A', cipher_len);

   auto step = make_pair((key - 1) << 1, 0);
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