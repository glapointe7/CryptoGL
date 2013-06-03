
#include "Adfgvx.hpp"

#include "Transposition.hpp"
#include "String.hpp"

#include "exceptions/EmptyGridKey.hpp"
#include "exceptions/BadGridDimension.hpp"
#include "exceptions/BadChar.hpp"

#include <utility>
#include <algorithm>

const std::string Adfgvx::code = "ADFGVX";

Adfgvx::Adfgvx(const KeyType &key)
   : SquareCipher(key)
{
   setAlpha(String::uppercase_digits);
}

void Adfgvx::setGridKey(const Grid &grid)
{
   if(grid.empty())
   {
      throw EmptyGridKey("Your grid key is empty.");
   }
   
   const uint8_t dimension = is6X6(grid);
   if(dimension != 6)
   {
      throw BadGridDimension("Your grid key should be of dimension 6 by 6.", dimension);
   }
   
   for(const auto row : grid)
   {
      const char c = badAlphaFound(row);
      if(c != 0)
      {
         throw BadChar("At least one character in your grid is not in your alphabet.", c);
      }
   }
   
   this->grid_key = grid;
}

const std::vector<int32_t> Adfgvx::getPermutationKey() const
{
   const KeyType key = getKey();
   std::string sorted_key(key);
   std::sort(sorted_key.begin(), sorted_key.end());
   
   std::vector<int32_t> perm_key;
   perm_key.reserve(key.length());
   
   for(const auto c : key)
   {
      perm_key.push_back(sorted_key.find(c));
   }
   
   return perm_key;
}

const Adfgvx::ClassicalType Adfgvx::encode(const ClassicalType &clear_text)
{
   if(grid_key.empty())
   {
      throw EmptyGridKey("Your grid key is not set.");
   }
   
   // Étape 1 : On prend les coordonnées de chaque lettre et on les remplace
   // par A,D,F,G,V ou X tels que A=0, D=1, F=2, G=3, V=4, X=5.
   // Exemple : Si la lettre 'K' se situe à (2,3), alors K s'encode FG.
   const KeyType key = getKey();
   ClassicalType first_encoding;
   first_encoding.reserve((clear_text.length() + key.length()) * 2);
   
   for (const auto c : clear_text)
   {
      const auto coords = getCharCoordinates(c, grid_key);

      first_encoding += code[coords.second];
      first_encoding += code[coords.first];
   }

   // Étape 2 : On surchiffre à l'aide d'une transposition avec la clé key.
   TranspositionCompleteColumns *TCol = new TranspositionCompleteColumns(getPermutationKey());
   
   return TCol->encode(first_encoding);
}

const Adfgvx::ClassicalType Adfgvx::decode(const ClassicalType &cipher_text)
{
   if(grid_key.empty())
   {
      throw EmptyGridKey("Your grid key is not set.");
   }
   
   const uint32_t cipher_len = cipher_text.length();
   ClassicalType decrypted;
   decrypted.reserve(cipher_len / 2);
   
   TranspositionCompleteColumns *TCol = new TranspositionCompleteColumns(getPermutationKey());
   ClassicalType first_decoding = TCol->decode(cipher_text);
   delete TCol;
   
   for(uint32_t i = 0; i < cipher_len; i += 2)
   {
      const auto coords = std::make_pair(code.find(first_decoding[i]), code.find(first_decoding[i+1]));
      decrypted += grid_key[coords.first][coords.second];
   }
   
   return decrypted;
}

// Check if the grid is 6X6.
uint8_t Adfgvx::is6X6(const Grid &grid)
{
   const uint8_t grid_size = grid.size();
   if(grid_size != 6)
   {
      return grid_size;
   }
   
   for(const auto row : grid)
   {
      const uint8_t row_size = row.length();
      if(row_size != 6)
      {
         return row_size;
      }
   }
   
   return grid_size;
}