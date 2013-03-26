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
        ~Wolseley() {};

        string encode() const;
        string decode() const;

        void setKey(const string key);
        void setAlpha(const string letters);
        
    private:
        string key;
};
#endif