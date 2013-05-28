/*
 * All classical ciphers that use a grid.
 */
#ifndef SQUARECIPHER_HPP
#define	SQUARECIPHER_HPP

#include "StringCipherWithStringKey.hpp"

#include <string>
#include <utility>
#include <vector>

class SquareCipher : public StringCipherWithStringKey
{
public:  
   SquareCipher(const KeyType &key); 
   
   virtual const ClassicalType encode(const ClassicalType &) = 0;
   virtual const ClassicalType decode(const ClassicalType &) = 0;

   void setGridDimension(const uint32_t dim);

protected:
   typedef std::pair<uint8_t, uint8_t> Coordinates;
   typedef std::vector<ClassicalType> Grid;
   
   const Grid getGrid(const ClassicalType &chars) const;
   static const Coordinates getCharCoordinates(const char c, const Grid &grid);

   // Dimension of the square grid.
   uint32_t dim;
};


#endif
