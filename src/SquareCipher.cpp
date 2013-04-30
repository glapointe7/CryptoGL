
#include "SquareCipher.hpp"

#include "Tools.hpp"

#include "EmptyKey.hpp"

void SquareCipher::setKey(const Key &key)
{
   if(key.empty())
   {
      throw EmptyKey("The Key should not be empty or not set.");
   }
   else
   {
      this->key = key;
   }
}

// La dimension doit être > 0 et non vide.
void SquareCipher::setGridDimension(const unsigned int dim)
{
   this->dim = dim;
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
   for (auto str : grid)
   {
      int32_t first = str.find(c);
      if (first != -1)
      {
         coords.first = static_cast<uint32_t>(first);
         break;
      }
      coords.second++;
   }
   
   return coords;
}
