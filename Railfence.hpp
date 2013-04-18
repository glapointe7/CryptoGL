#ifndef RAILFENCE_HPP
#define RAILFENCE_HPP

#include "StringCipher.hpp"
#include <string>

class Railfence : public StringCipher
{
public:
   Railfence();

   // Pour encoder avec un seul niveau pour tout le texte.
   virtual const ClassicalType encode(const ClassicalType &);
   virtual const ClassicalType decode(const ClassicalType &);

   void setKey(const unsigned int key);

private:
   unsigned int key;
};

#endif