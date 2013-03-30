
#ifndef PLAYFAIR_H
#define PLAYFAIR_H

#include "StringCipher.h"

class Playfair : public StringCipher
{
public:
   Playfair();

   std::string encode(const std::string &);
   std::string decode(const std::string &);

   void setKey(const std::string key);

private:
   std::string key;
   unsigned int cipher_len;
   unsigned int clear_len;

   const unsigned char rows = 5;
};

#endif // PLAYFAIR_H
