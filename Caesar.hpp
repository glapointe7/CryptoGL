#ifndef CAESAR_HPP
#define CAESAR_HPP

#include "StringCipher.h"

#include <string>

class Caesar : public StringCipher
{
public:
   virtual const ClassicalType encode(const ClassicalType &);
   virtual const ClassicalType decode(const ClassicalType &);

   void setKey(const unsigned char key);
   unsigned char getAlphaPosition(const char key, const char c) const;

private:
   unsigned char key;
};

#endif