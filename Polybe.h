
#ifndef POLYBE_H
#define POLYBE_H

#include "SquareCipher.h"

#include <string>

class Polybe : public SquareCipher
{
public:
   Polybe();

   virtual const ClassicalType encode(const ClassicalType &);
   virtual const ClassicalType decode(const ClassicalType &);
};

#endif
