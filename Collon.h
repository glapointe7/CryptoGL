
#ifndef COLLON_H
#define COLLON_H

#include "SquareCipher.h"

#include <string>

class Collon : public SquareCipher
{
public:
   Collon();

   virtual const ClassicalType encode(const ClassicalType &);
   virtual const ClassicalType decode(const ClassicalType &);

   void setBlockLength(const unsigned int series);

private:
   // Longueur des séries (bloc de caractères).
   unsigned int block_len;
};
#endif
