
#ifndef NIHILISTES_HPP
#define	NIHILISTES_HPP

#include "SquareCipher.hpp"

class Nihilistes : public SquareCipher
{
public:
   Nihilistes(const KeyType &key, const KeyType &second_key);
   
   virtual const ClassicalType encode(const ClassicalType &clear_text) final;
   virtual const ClassicalType decode(const ClassicalType &cipher_text) final;
   
private:
   KeyType second_key;
};

#endif