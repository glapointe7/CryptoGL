
#ifndef FOURSQUARES_HPP
#define FOURSQUARES_HPP

#include "SquareCipher.hpp"

class FourSquares : public SquareCipher
{
public:
   FourSquares(const KeyType &key1, const KeyType &key2);
   
   ClassicalType encode(const ClassicalType &clear_text) override;
   ClassicalType decode(const ClassicalType &cipher_text) override;

private:
   KeyType key2;
};

#endif