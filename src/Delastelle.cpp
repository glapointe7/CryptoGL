
#include "Delastelle.hpp"

#include "exceptions/BadAlphaLength.hpp"
#include "MathematicalTools.hpp"

#include <vector>
#include <utility>

void Delastelle::setBlockLength(const uint32_t block_len)
{
   this->block_len = block_len;
}

const Delastelle::ClassicalType Delastelle::encode(const ClassicalType &clear_text)
{
   if(block_len <= 0 || block_len > clear_text.length())
   {
      throw Exception("The block length you specified should be between 1 and the length of your message.");
   }
   
   if(!isPerfectSquare(alpha.length()))
   {
      throw BadAlphaLength("The length of your alphabet should be a perfect square.", alpha.length());
   }
   
   ClassicalType full_text(appendChars(clear_text, block_len, 'X'));
   const uint32_t clear_len = full_text.length();
   
   // Prendre chaque bloc de block_len caractères.
   Grid block;
   for (uint32_t i = 0; i < clear_len; i += block_len)
   {
      block.push_back(full_text.substr(i, block_len));
   }

   // Grille de chiffrement.
   const Grid grid(getGrid(getKey() + alpha));

   // Vecteur contenant les coordonnées en X des block_len caractères et celles en Y.
   std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char> > > coords_block;

   // Sous chaque lettre, on note les coordonnées des lettres verticalement.
   for (const auto str : block)
   {
      std::vector<unsigned char> X, Y;
      for (const auto c : str)
      {
         const auto coords = getCharCoordinates(c, grid);
         X.push_back(coords.first);
         Y.push_back(coords.second);
      }
      coords_block.push_back(std::make_pair(X, Y));
   }

   // Vecteur de toutes les coordonnées chiffrées.
   std::vector<std::pair<unsigned char, unsigned char> > encoded_coords;

   for (const auto XY : coords_block)
   {
      if ((block_len % 2) == 0)
      {
         // Les coordonnées en X obtenues.
         for (uint32_t i = 0; i < block_len; i += 2)
         {
            encoded_coords.push_back(std::make_pair(XY.first[i], XY.first[i + 1]));
         }

         // Les coordonnées en Y obtenues.
         for (uint32_t i = 0; i < block_len; i += 2)
         {
            encoded_coords.push_back(std::make_pair(XY.second[i], XY.second[i + 1]));
         }
      }
      else
      {
         for (uint32_t i = 0; i < block_len - 1; i += 2)
         {
            encoded_coords.push_back(std::make_pair(XY.second[i], XY.second[i + 1]));
         }
         encoded_coords.push_back(std::make_pair(XY.second[block_len - 1], XY.first[0]));
         for (uint32_t i = 1; i < block_len; i += 2)
         {
            encoded_coords.push_back(std::make_pair(XY.first[i], XY.first[i + 1]));
         }
      }
   }

   // On prend chaque coordonnées reçues et on retrouve le caractère associé dans la grille.
   ClassicalType crypted = "";
   crypted.reserve(clear_len);
   for (const auto xy : encoded_coords)
   {
      crypted += grid[xy.first][xy.second];
   }

   return crypted;
}

const Delastelle::ClassicalType Delastelle::decode(const ClassicalType &cipher_text)
{
   const uint32_t cipher_len = cipher_text.length();
   if(block_len <= 0 || block_len > cipher_len)
   {
      throw Exception("The block length you specified should be between 1 and the length of your message.");
   }
   
   if(!isPerfectSquare(alpha.length()))
   {
      throw BadAlphaLength("The length of your alphabet should be a perfect square.", alpha.length());
   }
   
   ClassicalType decrypted = "";
   decrypted.reserve(cipher_len);
   
   const Grid grid(getGrid(getKey() + alpha));
   std::vector<unsigned char> chars_coords;
   for(const auto c : cipher_text)
   {
      const auto coords = getCharCoordinates(c, grid);
      chars_coords.push_back(coords.second);
      chars_coords.push_back(coords.first);
   }
   
   
   for(uint32_t i = 0; i < cipher_len << 1; i += block_len << 1)
   {
      for(uint32_t k = 0; k < block_len; k++)
      {
         decrypted += grid[chars_coords[i+k]][chars_coords[i+k+block_len]];
      }
   }

   return decrypted;
}