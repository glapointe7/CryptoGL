
#include "Delastelle.hpp"

#include <vector>
#include <utility>

Delastelle::Delastelle()
{
   setAlpha("ABCDEFGHIJKLMNOPQRSTUVXYZ");
}

void Delastelle::setBlockLength(const unsigned int block_len)
{
   this->block_len = block_len;
}

const Delastelle::ClassicalType Delastelle::encode(const ClassicalType &clear_text)
{
   ClassicalType full_text(clear_text);
   full_text.append(block_len - (clear_text.length() % block_len), 'X');
   unsigned int clear_len = full_text.length();

   // Prendre chaque bloc de block_len caractères.
   Grid block;
   for (unsigned int i = 0; i < clear_len; i += block_len)
   {
      block.push_back(full_text.substr(i, block_len));
   }

   // Grille de chiffrement.
   Grid grid(getGrid(key + alpha));

   // Vecteur contenant les coordonnées en X des block_len caractères et celles en Y.
   std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char> > > coords_block;

   // Sous chaque lettre, on note les coordonnées des lettres verticalement.
   for (auto str : block)
   {
      std::vector<unsigned char> X, Y;
      for (auto c : str)
      {
         auto coords = getCharCoordinates(c, grid);
         X.push_back(coords.first);
         Y.push_back(coords.second);
      }
      coords_block.push_back(std::make_pair(X, Y));
   }

   // Vecteur de toutes les coordonnées chiffrées.
   std::vector<std::pair<unsigned char, unsigned char> > encoded_coords;

   for (auto XY : coords_block)
   {
      if ((block_len % 2) == 0)
      {
         // Les coordonnées en X obtenues.
         for (unsigned int i = 0; i < block_len; i += 2)
         {
            encoded_coords.push_back(std::make_pair(XY.first[i], XY.first[i + 1]));
         }

         // Les coordonnées en Y obtenues.
         for (unsigned int i = 0; i < block_len; i += 2)
         {
            encoded_coords.push_back(std::make_pair(XY.second[i], XY.second[i + 1]));
         }
      }
      else
      {
         for (unsigned int i = 0; i < block_len - 1; i += 2)
         {
            encoded_coords.push_back(std::make_pair(XY.second[i], XY.second[i + 1]));
         }
         encoded_coords.push_back(std::make_pair(XY.second[block_len - 1], XY.first[0]));
         for (unsigned int i = 1; i < block_len; i += 2)
         {
            encoded_coords.push_back(std::make_pair(XY.first[i], XY.first[i + 1]));
         }
      }
   }

   // On prend chaque coordonnées reçues et on retrouve le caractère associé dans la grille.
   ClassicalType crypted = "";
   crypted.reserve(clear_len);
   for (auto xy : encoded_coords)
   {
      crypted += grid[xy.first][xy.second];
   }

   return crypted;
}

const Delastelle::ClassicalType Delastelle::decode(const ClassicalType &cipher_text)
{
   unsigned int cipher_len = cipher_text.length();
   ClassicalType decrypted = "";
   decrypted.reserve(cipher_len);
   
   Grid grid(getGrid(key + alpha));
   std::vector<unsigned char > chars_coords;
   for(auto c : cipher_text)
   {
      auto coords = getCharCoordinates(c, grid);
      chars_coords.push_back(coords.second);
      chars_coords.push_back(coords.first);
   }
   
   
   for(unsigned int i = 0; i < cipher_len << 1; i += block_len << 1)
   {
      for(unsigned int k = 0; k < block_len; k++)
      {
         decrypted += grid[chars_coords[i+k]][chars_coords[i+k+block_len]];
      }
   }

   return decrypted;
}