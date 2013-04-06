
#include "Hill.hpp"

#include <utility>

#include "Tools.h"

Hill::Hill()
{

}

void Hill::setKey(const Matrix &key)
{
   this->key = key;
}

std::string Hill::encode(const std::string &clear_text)
{
   unsigned int clear_len = clear_text.length();
   unsigned int alpha_len = alpha.length();
   std::string crypted = "";
   crypted.reserve(clear_len << 1);

   std::string full_text(clear_text);
   if (clear_len % 2 == 1)
   {
      full_text.push_back('X');
      clear_len++;
   }

   for (unsigned int i = 0; i < clear_len - 1; i += 2)
   {
      auto pos = std::make_pair(alpha.find(full_text[i]), alpha.find(full_text[i + 1]));
      std::pair<long, long> bipos = std::make_pair((((long)key(1,1) * pos.first) + ((long)key(1,2) * pos.second)) % alpha_len,
              (((long)key(2,1) * pos.first) + ((long)key(2,2) * pos.second)) % alpha_len);

      if (bipos.first < 0)
      {
         bipos.first = getIntegerMod(bipos.first, alpha_len);
      }
      if (bipos.second < 0)
      {
         bipos.second = getIntegerMod(bipos.second, alpha_len);
      }
      crypted += alpha[bipos.first];
      crypted += alpha[bipos.second];
   }

   return crypted;
}

std::string Hill::decode(const std::string &cipher_text)
{
   unsigned int cipher_len = cipher_text.length();
   unsigned int alpha_len = alpha.length();
   std::string decrypted = "";
   Matrix inv_key = Matrix::inverse(key);

   for (unsigned int i = 0; i < cipher_len - 1; i += 2)
   {
      auto pos = std::make_pair(alpha.find(cipher_text[i]), alpha.find(cipher_text[i + 1]));
      std::pair<long, long> bipos = std::make_pair((((long)inv_key(1, 1) * pos.first) + ((long)inv_key(1, 2) * pos.second)) % alpha_len,
              (((long)inv_key(2, 1) * pos.first) + ((long)inv_key(2, 2) * pos.second)) % alpha_len);

      if (bipos.first < 0)
      {
         bipos.first = getIntegerMod(bipos.first, alpha_len);
      }
      if (bipos.second < 0)
      {
         bipos.second = getIntegerMod(bipos.second, alpha_len);
      }
      decrypted += alpha[bipos.first];
      decrypted += alpha[bipos.second];
   }
   
   return decrypted;
}