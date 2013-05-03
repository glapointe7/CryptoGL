
#include "SquareCipher.hpp"

#include "Tools.hpp"

#include "exceptions/EmptyKey.hpp"
#include "exceptions/BadChar.hpp"
#include "exceptions/BadGridDimension.hpp"

SquareCipher::SquareCipher()
{
   setAlpha(String::grid_uppercase_fr);
}

void SquareCipher::setKey(const Key &key)
{
   if(key.empty())
   {
      throw EmptyKey("The Key should not be empty or not set.");
   }
   else if(badAlphaFound(key))
   {
      throw BadChar("Your key contains at least one character that is not in your alphabet.");
   }
   else
   {
      this->key = key;
   }
}

// La dimension doit être > 0 et non vide.
void SquareCipher::setGridDimension(const unsigned int dim)
{
   if(dim <= 0)
   {
      throw BadGridDimension("The dimension of the grid should be greater than zero.");
   }
   else
   {
      this->dim = dim;
   }
}

// Construction de la grille de chiffrement.

const SquareCipher::Grid
SquareCipher::getGrid(const ClassicalType &chars) const
{
   const ClassicalType new_alpha(removeRepeatedLetters(chars));
   Grid grid;

   // Séparation de la chaîne pour former une grille carrée (dim X dim) de caractères.
   for (unsigned char i = 0; i < dim; ++i)
   {
      grid.push_back(new_alpha.substr(i * dim, dim));
   }

   return grid;
}

// Retourne les coordonées d'un caractère dans la grille de chiffrement.
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