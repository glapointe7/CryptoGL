
#ifndef POLYBE_HPP
#define POLYBE_HPP

#include "SquareCipher.hpp"

#include <string>

class Polybe : public SquareCipher
{
public:
   Polybe();

   const ClassicalType encode(const ClassicalType &) final;
   const ClassicalType decode(const ClassicalType &) final;
};

#endif