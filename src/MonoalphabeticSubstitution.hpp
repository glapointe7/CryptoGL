
#ifndef MONOALPHABETICSUBSTITUTION_HPP
#define	MONOALPHABETICSUBSTITUTION_HPP

#include "StringCipher.hpp"

class MonoalphabeticSubstitution : public StringCipher
{
public:
   virtual const ClassicalType encode(const ClassicalType &clear_text) final;
   virtual const ClassicalType decode(const ClassicalType &cipher_text) final;
   
   void setKey(const ClassicalType &key);

private:
   ClassicalType key;
};

#endif