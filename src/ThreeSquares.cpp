#include "ThreeSquares.hpp"

ThreeSquares::ThreeSquares(const KeyType &key1, const KeyType &key2, const KeyType &key3)
   : SquareCipher(key1)
{
   checkKey(key2);
   checkKey(key3);
   this->key2 = key2;
   this->key3 = key3;
}

const ClassicalType ThreeSquares::encode(const ClassicalType &clear_text)
{  
   const ClassicalType full_text(appendChars(clear_text, 2, 'X'));
   const uint32_t clear_len = full_text.length();
   ClassicalType crypted;
   crypted.reserve(clear_len + (clear_len / 2));

   const Grid grid1(getGrid(getKey() + alpha));
   const Grid grid2(getGrid(key2 + alpha));
   const Grid grid3(getGrid(key3 + alpha));
   
   srand (time(0));

   for (uint32_t i = 0; i < clear_len; i += 2)
   {
      const auto coord_grid1 = getCharCoordinates(clear_text[i], grid1);
      const auto coord_grid2 = getCharCoordinates(clear_text[i+1], grid2);
      const uint8_t random_index = rand() % dim;
      
      crypted.push_back(grid1[random_index][coord_grid1.first]);
      crypted.push_back(grid3[coord_grid1.second][coord_grid2.first]);
      crypted.push_back(grid2[coord_grid2.second][random_index]);
   }
   
   return crypted;
}

const ClassicalType ThreeSquares::decode(const ClassicalType &cipher_text)
{  
   const uint32_t cipher_len = cipher_text.length();
   ClassicalType decrypted;
   decrypted.reserve((2 * cipher_len) / 3);

   const Grid grid1(getGrid(getKey() + alpha));
   const Grid grid2(getGrid(key2 + alpha));
   const Grid grid3(getGrid(key3 + alpha));
   
   srand (time(0));

   for (uint32_t i = 0; i < cipher_len; i += 3)
   {
      const auto coord_grid1 = getCharCoordinates(cipher_text[i], grid1);
      const auto coord_grid2 = getCharCoordinates(cipher_text[i+2], grid2);
      const auto coord_grid3 = getCharCoordinates(cipher_text[i+1], grid3);
      
      decrypted.push_back(grid1[coord_grid3.second][coord_grid1.second]);
      decrypted.push_back(grid2[coord_grid2.first][coord_grid3.first]);
   }
   
   return decrypted;
}