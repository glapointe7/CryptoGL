#include "TwoSquares.hpp"

TwoSquares::TwoSquares(const KeyType &key1, const KeyType &key2)
   : SquareCipher(key1)
{
   checkKey(key2);
   this->key2 = key2;
}

ClassicalType TwoSquares::encode(const ClassicalType &clear_text)
{  
   const ClassicalType full_text = appendChars(clear_text, 2, 'X');
   const uint32_t clear_len = full_text.length();
   ClassicalType crypted;
   crypted.reserve(clear_len);

   const Grid grid1(getGrid(getKey() + alpha));
   const Grid grid2(getGrid(key2 + alpha));

   for (uint32_t i = 0; i < clear_len; i += 2)
   {
      const auto coord_grid1 = getCharCoordinates(clear_text[i], grid1);
      const auto coord_grid2 = getCharCoordinates(clear_text[i+1], grid2);
      
      // If the chars are on the same line, then there are encrypted by swapping them.
      if(coord_grid1.second != coord_grid2.second)
      {
         crypted.push_back(grid2[coord_grid1.second][coord_grid2.first]);
         crypted.push_back(grid1[coord_grid2.second][coord_grid1.first]);
      }
      else
      {
         crypted.push_back(clear_text[i+1]);
         crypted.push_back(clear_text[i]);
      }
   }

   return crypted;
}

ClassicalType TwoSquares::decode(const ClassicalType &cipher_text)
{  
   const uint32_t cipher_len = cipher_text.length();
   ClassicalType decrypted;
   decrypted.reserve(cipher_len);

   const Grid grid1(getGrid(getKey() + alpha));
   const Grid grid2(getGrid(key2 + alpha));

   for (uint32_t i = 0; i < cipher_len; i += 2)
   {
      const auto coord_grid1 = getCharCoordinates(cipher_text[i+1], grid1);
      const auto coord_grid2 = getCharCoordinates(cipher_text[i], grid2);
      
      // If the chars are on the same line, then there are encrypted by swapping them.
      if(coord_grid1.second != coord_grid2.second)
      {
         decrypted.push_back(grid1[coord_grid2.second][coord_grid1.first]);
         decrypted.push_back(grid2[coord_grid1.second][coord_grid2.first]);
      }
      else
      {
         decrypted.push_back(cipher_text[i+1]);
         decrypted.push_back(cipher_text[i]);
      }
   }

   return decrypted;
}