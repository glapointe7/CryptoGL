/* Modifié par Gabriel Lapointe
 * le 25 mars 2013
 */
#ifndef POLYBE_H
#define POLYBE_H

#include "StringCipher.h"
#include <vector>

class Polybe : public StringCipher
{
    public:
        Polybe();
        ~Polybe() {}

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