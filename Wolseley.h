/* 
 * File:   Wolseley.h
 * Author: gabriel
 *
 * Modified by Gabriel on 25 mars 2013
 */

#ifndef WOLSELEY_H
#define WOLSELEY_H

#include "StringCipher.h"

class Wolseley : public StringCipher
{
public:
   Wolseley();
   ~Wolseley();

   string encode();
   string decode();

   void setKey(const string key);

private:
   string key;
};
#endif