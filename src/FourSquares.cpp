#include "FourSquares.hpp"

FourSquares::FourSquares(const KeyType &key1, const KeyType &key2)
   : SquareCipher(key1)
{
   checkKey(key2);
   this->key2 = key2;
}

const ClassicalType FourSquares::encode(const ClassicalType &clear_text)
{  
   const ClassicalType full_text(appendChars(clear_text, 2, 'X'));
   const uint32_t clear_len = full_text.length();
   ClassicalType crypted;
   crypted.reserve(clear_len);

   // First 2 grids are simply the alphabet. Third and fourth use the keys.
   const Grid grid1(getGrid(alpha));
   const Grid grid2(grid1);
   const Grid grid3(getGrid(getKey() + alpha));
   const Grid grid4(getGrid(key2 + alpha));
   
   for (uint32_t i = 0; i < clear_len; i += 2)
   {
      const auto coord_grid1 = getCharCoordinates(clear_text[i], grid1);
      const auto coord_grid2 = getCharCoordinates(clear_text[i+1], grid2);
      crypted.push_back(grid3[coord_grid1.second][coord_grid2.first]);
      crypted.push_back(grid4[coord_grid2.second][coord_grid1.first]);
   }
   
   return crypted;
}

const ClassicalType FourSquares::decode(const ClassicalType &cipher_text)
{  
   const uint32_t cipher_len = cipher_text.length();
   ClassicalType decrypted;
   decrypted.reserve(cipher_len);

   // First 2 grids are simply the alphabet. Third and fourth use the keys.
   const Grid grid1(getGrid(alpha));
   const Grid grid2(grid1);
   const Grid grid3(getGrid(getKey() + alpha));
   const Grid grid4(getGrid(key2 + alpha));
   
   for (uint32_t i = 0; i < cipher_len; i += 2)
   {
      const auto coord_grid3 = getCharCoordinates(cipher_text[i], grid3);
      const auto coord_grid4 = getCharCoordinates(cipher_text[i+1], grid4);
      decrypted.push_back(grid1[coord_grid3.second][coord_grid4.first]);
      decrypted.push_back(grid2[coord_grid4.second][coord_grid3.first]);
   }
   
   return decrypted;
}