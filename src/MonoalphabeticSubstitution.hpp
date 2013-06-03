
#ifndef MONOALPHABETICSUBSTITUTION_HPP
#define	MONOALPHABETICSUBSTITUTION_HPP

#include "StringCipherWithStringKey.hpp"

class MonoalphabeticSubstitution : public StringCipherWithStringKey
{
public:
   explicit MonoalphabeticSubstitution(const KeyType &key) {setKey(key); }
   
   virtual const ClassicalType encode(const ClassicalType &clear_text) final;
   virtual const ClassicalType decode(const ClassicalType &cipher_text) final;
};

#endif