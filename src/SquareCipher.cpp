
#include "SquareCipher.hpp"

#include "Tools.hpp"
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
   if (!Maths::isPerfectSquare(alpha.length()))
   {
      throw BadAlphaLength("The length of your alphabet should be a perfect square.", alpha.length());
   }
   
   // get grid dimension from alphabet assuming length is a perfect square by the previous IF.
   dim = getByteSqrt(alpha.length());
   
   StringCipher::setAlpha(alpha);
}

// Construction of the cipher grid.

const SquareCipher::Grid
SquareCipher::getGrid(const ClassicalType &chars) const
{
   const ClassicalType new_alpha = String::makeUniqueChars(chars);
   Grid grid;
   grid.reserve(dim);

   // Split the string to set a square grid of dimension (dim X dim) of chars.
   for (uint8_t i = 0; i < dim; ++i)
   {
      grid.push_back(new_alpha.substr(i * dim, dim));
   }

   return grid;
}

// Return character coordinates from the cipher grid.
const SquareCipher::Coordinates
SquareCipher::getCharCoordinates(const char c, const Grid &grid) const
{
   auto coords = std::make_pair(0u, 0u);
   for (const auto str : grid)
   {
      const int32_t first = str.find(c);
      if (first != -1)
      {
         coords.first = static_cast<uint8_t>(first);
         break;
      }
      coords.second++;
   }
   
   return coords;
}