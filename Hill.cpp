
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
   /*unsigned int alpha_len = alpha.length();
   std::string crypted = "";
   crypted.reserve(clear_text.length() << 1);

   std::string full_text(clear_text);
   full_text.append(clear_text.length() % 2, 'X');
   unsigned int clear_len = full_text.length();

   for (unsigned int i = 0; i < clear_len - 1; i += 2)
   {
      auto pos = std::make_pair(alpha.find(full_text[i]), alpha.find(full_text[i + 1]));
      std::pair<long, long> bipos = std::make_pair(((key(1,1) * pos.first) + (key(1,2) * pos.second)) % alpha_len,
              ((key(2,1) * pos.first) + (key(2,2) * pos.second)) % alpha_len);

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

   return crypted;*/
   return "";
}

std::string Hill::decode(const std::string &cipher_text)
{
   /*unsigned int cipher_len = cipher_text.length();
   unsigned int alpha_len = alpha.length();
   std::string decrypted = "";
   //Eigen::MatrixXi inv_key = Eigen::MatrixXi::inverse(key);

   for (unsigned int i = 0; i < cipher_len - 1; i += 2)
   {
      auto pos = std::make_pair(alpha.find(cipher_text[i]), alpha.find(cipher_text[i + 1]));
      std::pair<long, long> bipos = std::make_pair(((inv_key(1, 1) * pos.first) + (inv_key(1, 2) * pos.second)) % alpha_len,
              ((inv_key(2, 1) * pos.first) + (inv_key(2, 2) * pos.second)) % alpha_len);

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
   
   return decrypted;*/
   return "";
}
