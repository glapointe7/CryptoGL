
#include "SquareCipher.hpp"

#include "String.hpp"
#include "MathematicalTools.hpp"

#include "exceptions/BadAlphaLength.hpp"

SquareCipher::SquareCipher(const KeyType &key) : dim(5)
{
   setAlpha(String::grid_uppercase_fr);
   setKey(key);
}

void SquareCipher::setAlpha(const ClassicalType &alpha)
{
   const uint8_t alpha_size = alpha.length();
   if (!Maths::isPerfectSquare(alpha_size))
   {
      throw BadAlphaLength("The length of your alphabet should be a perfect square.", alpha_size);
   }
   
   // get grid dimension from alphabet assuming length is a perfect square by the previous IF.
   dim = getByteSqrt(alpha_size);
   
   StringCipher::setAlpha(alpha);
}

// Construction of the cipher grid.

Grid SquareCipher::getGrid(ClassicalType chars) const
{
   chars = String::makeUniqueChars(chars);
   Grid grid;
   grid.reserve(dim);

   // Split the string to set a square grid of dimension (dim X dim) of chars.
   for (uint8_t i = 0; i < dim; ++i)
   {
      grid.push_back(chars.substr(i * dim, dim));
   }

   return grid;
}

// Return character coordinates from the cipher grid.
Coordinates SquareCipher::getCharCoordinates(const char c, const Grid &grid)
{
   Coordinates coords(0, 0);
   for (const auto &str : grid)
   {
      const std::string::size_type found = str.find(c);
      if (found != std::string::npos)
      {
         coords.first = found;
         return coords;
      }
      coords.second++;
   }
   
   return coords;
}