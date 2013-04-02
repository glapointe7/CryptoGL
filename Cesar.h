
#ifndef CESAR_H
#define CESAR_H

#include "StringCipher.h"

#include <string>

class Cesar : public StringCipher
{
public:
   Cesar();

   std::string encode(const std::string &);
   std::string decode(const std::string &);

   void setKey(const unsigned char key);
   unsigned char getAlphaPosition(const char key, const char c) const;

private:
   unsigned char key;
};

#endif // CESAR_H
