/* 
 * File:   SquareCipher.cpp
 * Author: gabriel
 * 
 * Created on March 30, 2013, 1:42 PM
 */

#include "SquareCipher.h"

#include "Tools.h"

#include <vector>
#include <string>
#include <utility>

using namespace std;

SquareCipher::SquareCipher()
{
}

void SquareCipher::setKey(const string &key)
{
   this->key = key;
}

void SquareCipher::setGridDimension(const unsigned char dim)
{
   this->dim = dim;
}

// Construction de la grille de chiffrement.

vector<string> SquareCipher::getGrid(const string &chars) const
{
   string new_alpha(removeRepeatedLetters(chars));
   vector<string> grid;

   // Séparation de la chaîne pour former une grille carrée (dim X dim) de caractères.
   for (unsigned char i = 0; i < dim; i++)
   {
      grid.push_back(new_alpha.substr(i * dim, dim));
   }

   return grid;
}

// Retourne les coordonées d'un caractère dans la grille de chiffrement.
Coordinates SquareCipher::getCharCoordinates(const char c, const vector<string> &grid) const
{
   auto coords = make_pair(0, 0);
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