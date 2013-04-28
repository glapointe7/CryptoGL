
#ifndef WOLSELEY_HPP
#define WOLSELEY_HPP

#include "StringCipher.hpp"

#include <string>

class Wolseley : public StringCipher
{
public:
   Wolseley();

   const ClassicalType encode(const ClassicalType &) final;
   const ClassicalType decode(const ClassicalType &) final;

   void setKey(const std::string &key);

private:
   std::string key;
};

#endif