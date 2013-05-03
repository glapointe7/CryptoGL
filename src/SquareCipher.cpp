
#include "SquareCipher.hpp"

#include "Tools.hpp"

#include "exceptions/EmptyKey.hpp"
#include "exceptions/BadChar.hpp"
#include "exceptions/BadGridDimension.hpp"

SquareCipher::SquareCipher() : dim(5)
{
   setAlpha(String::grid_uppercase_fr);
}

void SquareCipher::setKey(const Key &key)
{
   if(key.empty())
   {
      throw EmptyKey("The Key should not be empty or not set.");
   }
   
   const char c = badAlphaFound(key);
   if(c != 0)
   {
      throw BadChar("Your key contains at least one character that is not in your alphabet.", c);
   }
      
   this->key = key;
}

void SquareCipher::setGridDimension(const unsigned int dim)
{
   if(dim <= 0)
   {
      throw BadGridDimension("The dimension of the grid have to be greater than zero.", dim);
   }
   
   this->dim = dim;
}

// Construction of the cipher grid.

const SquareCipher::Grid
SquareCipher::getGrid(const ClassicalType &chars) const
{
   const ClassicalType new_alpha(removeRepeatedLetters(chars));
   Grid grid;

   // Split the string to set a square grid of dimension (dim X dim) of chars.
   for (unsigned char i = 0; i < dim; ++i)
   {
      grid.push_back(new_alpha.substr(i * dim, dim));
   }

   return grid;
}

// Return character coordinates from the cipher grid.
const SquareCipher::Coordinates
SquareCipher::getCharCoordinates(const char c, const Grid &grid)
{
   auto coords = std::make_pair(0u, 0u);
   for (const auto str : grid)
   {
      const int32_t first = str.find(c);
      if (first != -1)
      {
         coords.first = static_cast<uint32_t>(first);
         break;
      }
      coords.second++;
   }
   
   return coords;
}