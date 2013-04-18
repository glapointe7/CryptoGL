
#ifndef POLYBE_HPP
#define POLYBE_HPP

#include "SquareCipher.hpp"

#include <string>

class Polybe : public SquareCipher
{
public:
   Polybe();

   virtual const ClassicalType encode(const ClassicalType &);
   virtual const ClassicalType decode(const ClassicalType &);
};

#endif
