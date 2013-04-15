
#include "SquareCipher.h"

#include "Tools.h"

void SquareCipher::setKey(const Key &key)
{
   this->key = key;
}

void SquareCipher::setGridDimension(const unsigned int dim)
{
   this->dim = dim;
}

// Construction de la grille de chiffrement.

const SquareCipher::Grid
SquareCipher::getGrid(const ClassicalType &chars) const
{
   ClassicalType new_alpha(removeRepeatedLetters(chars));
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
   auto coords = std::make_pair(0, 0);
   for (auto str : grid)
   {
      coords.first = str.find(c);
      if (coords.first != std::string::npos)
      {
         break;
      }
      coords.second++;
   }
   
   return coords;
}
