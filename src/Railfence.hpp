#ifndef RAILFENCE_HPP
#define RAILFENCE_HPP

#include "StringCipher.hpp"
#include <string>

class Railfence : public StringCipher
{
public:
   virtual const ClassicalType encode(const ClassicalType &);
   virtual const ClassicalType decode(const ClassicalType &);

   void setKey(const unsigned int key);

private:
   unsigned int key;
};

#endif