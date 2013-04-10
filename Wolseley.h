
#ifndef WOLSELEY_H
#define WOLSELEY_H

#include "StringCipher.h"

#include <string>

class Wolseley : public StringCipher
{
public:
   Wolseley();

   virtual std::string encode(const std::string &);
   virtual std::string decode(const std::string &);

   void setKey(const std::string &key);

private:
   std::string key;
};

#endif // WOLSELEY_H
