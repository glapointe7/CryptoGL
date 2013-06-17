
#include "Delastelle.hpp"

#include "MathematicalTools.hpp"

#include <vector>
#include <utility>

void Delastelle::setBlockLength(const uint32_t block_len)
{
   if (block_len == 0)
   {
      throw ZeroBlockLength("The block length you provided has to be greater than zero.");
   }

   this->block_len = block_len;
}

const Delastelle::ClassicalType Delastelle::encode(const ClassicalType &clear_text)
{
   const ClassicalType full_text(appendChars(clear_text, block_len, 'X'));
   const uint32_t clear_len = full_text.length();

   // Separate the text in blocks of length chosen by block_len.
   const uint32_t block_size = clear_len / block_len;
   Grid block;
   block.reserve(block_size);
   for (uint32_t i = 0; i < clear_len; i += block_len)
   {
      block.push_back(full_text.substr(i, block_len));
   }

   // Fill the cipher grid.
   const Grid grid = getGrid(getKey() + alpha);
   
   // Sous chaque lettre, on note les coordonnées des lettres verticalement.
   ClassicalType crypted;
   crypted.reserve(clear_len);
   for (const auto str : block)
   {
      std::vector<uint8_t> X, Y;
      X.reserve(block_len);
      Y.reserve(block_len);
      for (const auto c : str)
      {
         const auto coords = getCharCoordinates(c, grid);
         X.push_back(coords.first);
         Y.push_back(coords.second);
      }

      if ((block_len % 2) == 0)
      {
         // Y coordinates.
         for (uint32_t i = 0; i < block_len; i += 2)
         {
            crypted.push_back(grid[Y[i]][Y[i+1]]);
         }

         // X coordinates.
         for (uint32_t i = 0; i < block_len; i += 2)
         {
            crypted.push_back(grid[X[i]][X[i+1]]);
         }
      }
      else
      {
         for (uint32_t i = 0; i < block_len - 1; i += 2)
         {
            crypted.push_back(grid[Y[i]][Y[i+1]]);
         }
         crypted.push_back(grid[Y[block_len-1]][X[0]]);
         for (uint32_t i = 1; i < block_len; i += 2)
         {
            crypted.push_back(grid[X[i]][X[i+1]]);
         }
      }
   }

   return crypted;
}

const Delastelle::ClassicalType Delastelle::decode(const ClassicalType &cipher_text)
{
   const uint32_t cipher_len = cipher_text.length() * 2;
   ClassicalType decrypted;
   decrypted.reserve(cipher_len / 2);

   const Grid grid(getGrid(getKey() + alpha));
   std::vector<uint8_t> chars_coords;
   chars_coords.reserve(cipher_len);
   for (const auto c : cipher_text)
   {
      const auto coords = getCharCoordinates(c, grid);
      chars_coords.push_back(coords.second);
      chars_coords.push_back(coords.first);
   }

   const uint32_t double_block_len = block_len << 1;
   for (uint32_t i = 0; i < cipher_len; i += double_block_len)
   {
      for (uint32_t k = 0; k < block_len; ++k)
      {
         decrypted.push_back(grid[chars_coords[i + k]][chars_coords[i + k + block_len]]);
      }
   }

   return decrypted;
}