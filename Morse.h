#ifndef MORSE_H
#define MORSE_H

#include "StringCipher.h"
#include <vector>

class Morse : public StringCipher
{
    public:
        Morse();
        ~Morse() {};

        string encode();
        string decode();
        
        void setAlpha(const string letters);
    
    private:
        vector<string> morse;
};
#endif