
#ifndef PLAYFAIR_H
#define PLAYFAIR_H

#include "SquareCipher.h"

#include <string>

class Playfair : public SquareCipher
{
public:
   Playfair();

   virtual const ClassicalType encode(const ClassicalType &);
   virtual const ClassicalType decode(const ClassicalType &);
};

#endif