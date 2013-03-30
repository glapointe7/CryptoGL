
#ifndef POLYBE_H
#define POLYBE_H

#include "StringCipher.h"

#include <vector>

class Polybe : public StringCipher
{
public:
   Polybe();

   std::string encode(const std::string &);
   std::string decode(const std::string &);

   void setKey(const std::string &key);

private:
   std::string key;
   unsigned int cipher_len;
   unsigned int clear_len;

   const unsigned char rows = 5;
};

#endif // POLYBE_H
