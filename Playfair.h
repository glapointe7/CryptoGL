/* Modifié par Gabriel Lapointe
 * le 25 mars 2013
 */

#ifndef PLAYFAIR_H
#define PLAYFAIR_H

#include "StringCipher.h"

class Playfair : public StringCipher
{
    public:
        Playfair();
        ~Playfair() {}

        string encode() const;
        string decode() const;
        
        void setKey(const string key);
        void setAlpha(const string letters);
    
    private:
		string key;
        unsigned int cipher_len;
        unsigned int clear_len;
        
        const unsigned char rows = 5;
};
#endif