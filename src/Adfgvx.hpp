
#ifndef ADFGVX_HPP
#define ADFGVX_HPP

#include "SquareCipher.hpp"
#include <vector>
#include <string>

class Adfgvx : public SquareCipher
{
public:
   explicit Adfgvx(const KeyType &key);

   ClassicalType encode(const ClassicalType &clear_text) override;
   ClassicalType decode(const ClassicalType &cipher_text) override;

   void setGridKey(const Grid &grid);

private:
   static uint8_t is6X6(const Grid &grid);
   Int32Vector getPermutationKey() const;
   
   // 6X6 grid key.
   Grid grid_key;

   static const ClassicalType code;
};

#endif