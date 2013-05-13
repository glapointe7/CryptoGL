
#include "Fleissner.hpp"

#include <set>
#include <time.h>
#include <vector>

// The key is represented by the coordinates of each 'hole' in the grid's mask.

void Fleissner::setKey(const std::vector<Coordinates> key)
{
   this->key = key;
}

void Fleissner::setGridDimension(const unsigned short dim)
{
   grid_dim = dim;
}

// Check if the initial mask holes represent exactly the quarter of the number of cells grid.

bool Fleissner::checkMask(std::vector<Coordinates> &coords) const
{
   const unsigned short key_size = key.size();
   const unsigned short mask_size_approuved = key_size << 2;

   // The gris has to be square.
   if (mask_size_approuved != (grid_dim * grid_dim))
   {
      return false;
   }

   // Sort coordinates for each rotation.
   std::set<Coordinates> rotation90, rotation180, rotation270;
   std::set<Coordinates> cmp;
   std::pair < std::set<Coordinates>::iterator, bool> is_unique;

   // Make sure that the coordinates are unique.
   for (const auto xy : key)
   {
      cmp.insert(std::make_pair(xy.first, xy.second));
   }

   // If the rotation coordinates exist, then the mask is not valid. 
   for (const auto xy : key)
   {
      // 270 degrees
      is_unique = cmp.insert(std::make_pair(grid_dim - 1 - xy.second, xy.first));
      if (is_unique.second == false)
      {
         return false;
      }
      rotation270.insert(std::make_pair(grid_dim - 1 - xy.second, xy.first));
   }
   coords.insert(coords.end(), rotation270.begin(), rotation270.end());

   for (const auto xy : key)
   {
      // 180 degrees
      is_unique = cmp.insert(std::make_pair(grid_dim - 1 - xy.first, grid_dim - 1 - xy.second));
      if (is_unique.second == false)
      {
         return false;
      }
      rotation180.insert(std::make_pair(grid_dim - 1 - xy.first, grid_dim - 1 - xy.second));
   }
   coords.insert(coords.end(), rotation180.begin(), rotation180.end());

   for (const auto xy : key)
   {
      // 90 degrees
      is_unique = cmp.insert(std::make_pair(xy.second, grid_dim - 1 - xy.first));
      if (is_unique.second == false)
      {
         return false;
      }
      rotation90.insert(std::make_pair(xy.second, grid_dim - 1 - xy.first));
   }
   coords.insert(coords.end(), rotation90.begin(), rotation90.end());

   return true;
}

const Fleissner::ClassicalType Fleissner::encode(const ClassicalType &clear_text)
{
   ClassicalType crypted = "";
   std::vector<Coordinates> coords(key);
   unsigned short dim = grid_dim * grid_dim;

   if (checkMask(coords) == true)
   {
      ClassicalType full_text(appendChars(clear_text, dim, 'X'));

      // On réserve l'espace selon grid_dim pour la grille.
      std::vector<std::string> grid;
      grid.reserve(grid_dim);
      for (unsigned short p = 0; p < grid_dim; p++)
      {
         grid[p].reserve(grid_dim);
      }

      // If the grid is filled, we clear it and we start the process again.
      const unsigned int clear_len = full_text.length();
      const unsigned short max_grid = static_cast<unsigned short> (clear_len / dim);
      unsigned int k = 0;

      for (unsigned short i = 0; i < max_grid; i++)
      {
         for (unsigned short j = 0; j < dim; j++, k++)
         {
            grid[coords[j].first][coords[j].second] = full_text[k];
         }

         for (const auto str : grid)
         {
            crypted += str;
         }
         grid.clear();
      }
   }

   return crypted;
}

const Fleissner::ClassicalType Fleissner::decode(const ClassicalType &cipher_text)
{
   ClassicalType decrypted = "";
   decrypted.reserve(cipher_text.length());
   std::vector<Coordinates> coords(key);
   const unsigned short dim = grid_dim * grid_dim;

   if (checkMask(coords) == true)
   {
      const unsigned short max_grid = static_cast<unsigned short> (cipher_text.length() / dim);
      unsigned int k = 0;
      std::vector<std::string> grid;

      for (unsigned int i = 0; i < max_grid; i++)
      {
         // Fill the grid with the cipher text.
         for (unsigned short j = 0; j < grid_dim; j++)
         {
            grid.push_back(cipher_text.substr(k, grid_dim));
            k += grid_dim;
         }

         for (unsigned int c = 0; c < dim; c++)
         {
            decrypted += grid[coords[c].first][coords[c].second];
         }
      }
   }

   return decrypted;
}