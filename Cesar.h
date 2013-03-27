#ifndef CESAR_H
#define CESAR_H

#include "StringCipher.h"

class Cesar : public StringCipher
{
public:
   Cesar();
   ~Cesar();

   string encode() const;
   string decode() const;

   void setKey(const unsigned char key);
   unsigned char getAlphaPosition(const char key, const char c) const;

private:
   unsigned char key;
};
#endif