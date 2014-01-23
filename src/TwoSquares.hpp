
#ifndef TWOSQUARES_HPP
#define TWOSQUARES_HPP

#include "SquareCipher.hpp"

class TwoSquares : public SquareCipher
{
public:
   TwoSquares(const KeyType &key1, const KeyType &key2);
   
   ClassicalType encode(const ClassicalType &clear_text) override;
   ClassicalType decode(const ClassicalType &cipher_text) override;

private:
   KeyType key2;
};

#endif