
#ifndef FOURSQUARES_HPP
#define FOURSQUARES_HPP

#include "SquareCipher.hpp"
#include "CipherGrid.hpp"

class FourSquares : public SquareCipher
{
public:
   FourSquares(const KeyType &key1, const KeyType &key2, const ClassicalType &alpha);
   
   FourSquares(const KeyType &key1, const KeyType &key2) 
      : FourSquares(key1, key2, ClassicalType::grid_uppercase_fr) {}
   
   ClassicalType encode(const ClassicalType &clear_text) override;
   ClassicalType decode(const ClassicalType &cipher_text) override;

private:
   KeyType key2;
   const CipherGrid grid2;
};

#endif