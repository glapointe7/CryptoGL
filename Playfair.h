
#ifndef PLAYFAIR_H
#define PLAYFAIR_H

#include "SquareCipher.h"

#include <string>

class Playfair : public SquareCipher
{
public:
   Playfair();

   virtual std::string encode(const std::string &);
   virtual std::string decode(const std::string &);
};

#endif // PLAYFAIR_H
