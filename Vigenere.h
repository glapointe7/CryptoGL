/* 
 * File:   clsVigenere.h
 * Author: gabriel
 *
 * Created on March 29, 2013, 8:32 AM
 */

#ifndef VIGENERE_H
#define	VIGENERE_H

#include "StringCipher.h"

class Vigenere : public StringCipher
{
public:
   Vigenere();
   Vigenere(const Vigenere& orig);
   ~Vigenere();
   
   string encode() const;
   string decode() const;
   
   void setKey(const string key);
   
private:
   string key;

};

#endif	/* CLSVIGENERE_H */

