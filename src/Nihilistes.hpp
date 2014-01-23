
#ifndef NIHILISTES_HPP
#define NIHILISTES_HPP

#include "SquareCipher.hpp"

class Nihilistes : public SquareCipher
{
public:
   Nihilistes(const KeyType &key, const KeyType &second_key);
   
   ClassicalType encode(const ClassicalType &clear_text) override;
   ClassicalType decode(const ClassicalType &cipher_text) override;
   
private:
   KeyType second_key;
};

#endif