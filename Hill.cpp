
#include "Hill.hpp"

#include <utility>

#include "Tools.h"

using namespace std;

Hill::Hill()
{

}

void Hill::setKey(const vector<vector<long> > &key)
{
   this->key = key;
}

string Hill::encode(const std::string &clear_text)
{
   unsigned int clear_len = clear_text.length();
   unsigned int alpha_len = alpha.length();
   string crypted = "";
   crypted.reserve(clear_len << 1);

   string full_text(clear_text);
   if (clear_len % 2 == 1)
   {
      full_text.push_back('X');
      clear_len++;
   }

   for (unsigned int i = 0; i < clear_len - 1; i += 2)
   {
      auto pos = make_pair(alpha.find(full_text[i]), alpha.find(full_text[i + 1]));
      pair<long, long> bipos = make_pair(((key[0][0] * pos.first) + (key[0][1] * pos.second)) % alpha_len,
              ((key[1][0] * pos.first) + (key[1][1] * pos.second)) % alpha_len);

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

string Hill::decode(const std::string &cipher_text)
{
   unsigned int cipher_len = cipher_text.length();
   unsigned int alpha_len = alpha.length();
   string decrypted = "";
   //buildInverse();

   for (unsigned int i = 0; i < cipher_len - 1; i += 2)
   {
      auto pos = make_pair(alpha.find(cipher_text[i]), alpha.find(cipher_text[i + 1]));
      pair<long, long> bipos = make_pair(((key[0][0] * pos.first) + (key[0][1] * pos.second)) % alpha_len,
              ((key[1][0] * pos.first) + (key[1][1] * pos.second)) % alpha_len);

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