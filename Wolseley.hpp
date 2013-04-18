
#ifndef WOLSELEY_HPP
#define WOLSELEY_HPP

#include "StringCipher.hpp"

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