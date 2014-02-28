#include "FourSquares.hpp"

FourSquares::FourSquares(const KeyType &key1, const KeyType &key2, const ClassicalType &alpha)
   : SquareCipher(key1, alpha), grid2(key2, alpha)
{
   checkKey(key2);
   this->key2 = key2;
}

ClassicalType FourSquares::encode(const ClassicalType &clear_text)
{  
   const ClassicalType full_text = appendChars(clear_text, 2, 'X');
   const uint32_t clear_len = full_text.length();
   ClassicalType crypted(clear_len);

   const CipherGrid grid3(alpha);
   const CipherGrid grid4(grid3);
   
   for (uint32_t i = 0; i < clear_len; i += 2)
   {
      const auto coord_grid1 = grid3.getCharCoordinates(clear_text[i]);
      const auto coord_grid2 = grid4.getCharCoordinates(clear_text[i+1]);
      crypted.push_back(grid.at(coord_grid1.second, coord_grid2.first));
      crypted.push_back(grid2.at(coord_grid2.second, coord_grid1.first));
   }
   
   return crypted;
}

ClassicalType FourSquares::decode(const ClassicalType &cipher_text)
{  
   const uint32_t cipher_len = cipher_text.length();
   ClassicalType decrypted(cipher_len);

   const CipherGrid grid3(alpha);
   const CipherGrid grid4(grid3);
   
   for (uint32_t i = 0; i < cipher_len; i += 2)
   {
      const auto coord_grid3 = grid.getCharCoordinates(cipher_text[i]);
      const auto coord_grid4 = grid2.getCharCoordinates(cipher_text[i+1]);
      decrypted.push_back(grid3.at(coord_grid3.second, coord_grid4.first));
      decrypted.push_back(grid4.at(coord_grid4.second, coord_grid3.first));
   }
   
   return decrypted;
}