#include "Railfence.hpp"
#include "exceptions/BadKeyLength.hpp"

#include <string>
#include <utility>

void Railfence::setKey(const unsigned int key)
{
   if(key == 0)
   {
      throw BadKeyLength("Your key have to be greater than zero.", 0);
   }
   
   this->key = key;
}


const Railfence::ClassicalType Railfence::encode(const ClassicalType &clear_text)
{
   const unsigned int clear_len = clear_text.length();
   ClassicalType crypted = "";
   crypted.reserve(clear_len);
   const unsigned int n = (key - 1) << 1;

   for (unsigned short i = 0; i < key; i++)
   {
      for (unsigned int j = 0; j < clear_len; j++)
      {
         if (j % n == i || j % n == n - i)
         {
            crypted += alpha[alpha.find(clear_text[j])];
         }
      }
   }

   return crypted;
}


const Railfence::ClassicalType Railfence::decode(const ClassicalType &cipher_text)
{
   const unsigned int cipher_len = cipher_text.length();
   ClassicalType decrypted(cipher_len, 'A');

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