
#ifndef COLLON_H
#define COLLON_H

#include "SquareCipher.h"

#include <string>

class Collon : public SquareCipher
{
public:
   Collon();

   virtual std::string encode(const std::string &);
   virtual std::string decode(const std::string &);

   void setBlockLength(const unsigned int series);

private:
   // Longueur des séries (bloc de caractères).
   unsigned int block_len;
};
#endif // COLLON_H
