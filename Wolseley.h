
#ifndef WOLSELEY_H
#define WOLSELEY_H

#include "StringCipher.h"

#include <string>

class Wolseley : public StringCipher
{
public:
   Wolseley();

   virtual const ClassicalType encode(const ClassicalType &);
   virtual const ClassicalType decode(const ClassicalType &);

   void setKey(const std::string &key);

private:
   std::string key;
};

#endif