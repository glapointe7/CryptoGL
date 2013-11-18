/*
 * All string ciphers that uses a square grid.
 */
#ifndef SQUARECIPHER_HPP
#define SQUARECIPHER_HPP

#include "StringCipherWithStringKey.hpp"

#include <string>
#include <utility>
#include <vector>

class SquareCipher : public StringCipherWithStringKey
{
protected:
   using Coordinates = std::pair<uint8_t, uint8_t>;
   using Grid = std::vector<ClassicalType>;
   
   explicit SquareCipher(const KeyType &key); 
   virtual ~SquareCipher() {}
   
   virtual ClassicalType encode(const ClassicalType &) = 0;
   virtual ClassicalType decode(const ClassicalType &) = 0;

   virtual void setAlpha(const ClassicalType &alpha) final;
   
   Grid getGrid(const ClassicalType &chars) const;
   Coordinates getCharCoordinates(const char c, const Grid &grid) const;

   // Dimension of the square grid.
   uint8_t dim;
};

#endif