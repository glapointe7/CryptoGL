
#include "Adfgvx.hpp"

#include "Transposition.hpp"
#include "exceptions/EmptyGridKey.hpp"
#include "exceptions/BadGridDimension.hpp"
#include "exceptions/BadChar.hpp"

#include <utility>

const std::string Adfgvx::code = "ADFGVX";

Adfgvx::Adfgvx()
{
   setAlpha("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
}

// La grille ne doit pas être vide.
// La grille doit être de 6 X 6.
// La grille ne doit pas contenir d'autres caractères que ceux d'alpha.
void Adfgvx::setGridKey(const Grid &grid)
{
   if(grid.empty())
   {
      throw EmptyGridKey("Your grid key is empty.");
   }
   
   if(!is6X6(grid))
   {
      throw BadGridDimension("Your grid key should be of dimension 6 by 6.");
   }
   
   for(const auto row : grid)
   {
      if(badAlphaFound(row))
      {
         throw BadChar("At least one character in your grid is not in your alphabet.");
      }
   }
   
   this->grid_key = grid;
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
   std::string first_encoding = "";
   first_encoding.reserve((clear_text.length() + key.length()) << 1);
   
   for (const auto c : clear_text)
   {
      const auto coords = getCharCoordinates(c, grid_key);

      first_encoding += code[coords.second];
      first_encoding += code[coords.first];
   }

   // Étape 2 : On surchiffre à l'aide d'une transposition avec la clé key.
   TranspositionColumns *TCol = new TranspositionColumns();
   TCol->setKey(key);
   
   return TCol->encode(first_encoding);
}

const Adfgvx::ClassicalType Adfgvx::decode(const ClassicalType &cipher_text)
{
   if(grid_key.empty())
   {
      throw EmptyGridKey("Your grid key is not set.");
   }
   
   const unsigned int cipher_len = cipher_text.length();
   ClassicalType decrypted = "";
   decrypted.reserve(cipher_len >> 1);
   
   TranspositionColumns *TCol = new TranspositionColumns();
   TCol->setKey(key);
   const std::string first_decoding = TCol->decode(cipher_text);
   
   for(unsigned int i = 0; i < cipher_len; i += 2)
   {
      const auto coords = std::make_pair(code.find(first_decoding[i]), code.find(first_decoding[i+1]));
      decrypted += grid_key[coords.first][coords.second];
   }
   
   return decrypted;
}

// Vérifie si la grille est de dimension 6X6.
const bool Adfgvx::is6X6(const Grid &grid)
{
   if(grid.size() != 6)
   {
      return false;
   }
   
   for(const auto row : grid)
   {
      if(row.size() != 6)
      {
         return false;
      }
   }
   
   return true;
}