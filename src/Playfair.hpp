
#ifndef PLAYFAIR_HPP
#define PLAYFAIR_HPP

#include "SquareCipher.hpp"

#include <string>

class Playfair : public SquareCipher
{
public:
   virtual const ClassicalType encode(const ClassicalType &);
   virtual const ClassicalType decode(const ClassicalType &);
};

#endif