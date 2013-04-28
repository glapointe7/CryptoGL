
#ifndef COLLON_HPP
#define COLLON_HPP

#include "SquareCipher.hpp"

#include <string>

class Collon : public SquareCipher
{
public:
   Collon();

   const ClassicalType encode(const ClassicalType &) final;
   const ClassicalType decode(const ClassicalType &) final;

   void setBlockLength(const unsigned int series);

private:
   // Longueur des séries (bloc de caractères).
   unsigned int block_len;
};
#endif
