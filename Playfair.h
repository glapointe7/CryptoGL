
#ifndef PLAYFAIR_H
#define PLAYFAIR_H

#include "SquareCipher.h"

#include <string>

class Playfair : public SquareCipher
{
public:
   Playfair();

   std::string encode(const std::string &);
   std::string decode(const std::string &);

private:

};

#endif // PLAYFAIR_H
