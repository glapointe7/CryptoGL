
#include "Playfair.hpp"

#include <utility>

Playfair::Playfair()
{
   setAlpha("ABCDEFGHIJKLMNOPQRSTUVXYZ");
}

// Encode un texte clair avec la méthode de Playfair.

const Playfair::ClassicalType Playfair::encode(const ClassicalType &clear_text)
{
   ClassicalType full_text(clear_text);
   if (clear_text.length() % 2 != 0)
   {
      full_text += "X";
   }
   const unsigned int clear_len = full_text.length();
   ClassicalType crypted = "";
   crypted.reserve(clear_len);

   const Grid grid(getGrid(key + alpha));

   for (unsigned int i = 0; i < clear_len; i += 2)
   {
      // Obtenir les coordonnées (x,y) et (a,b) des lettres du bigramme dans la grille.
      const auto X = getCharCoordinates(full_text[i], grid);
      const auto A = getCharCoordinates(full_text[i+1], grid);

      // Soient X = (x,y) et A = (a,b).
      // R�gle 1 : Si x != a ET y != b, alors X = (x,b) et A = (a,y).
      if (X.first != A.first && A.second != X.second)
      {
         crypted += grid[A.first][X.second];
         crypted += grid[X.first][A.second];
      }
         // R�gle 2 : Si x = a ET y != b, alors X = (x+1,y) et A = (a+1,b).
      else if (X.first == A.first && X.second != A.second)
      {
         crypted += grid[X.first][(X.second + 1) % dim];
         crypted += grid[A.first][(A.second + 1) % dim];
      }
         // R�gle 3 : Si x != a ET y = b, alors X = (x,y+1) et A = (a,b+1).
      else if (X.first != A.first && A.second == X.second)
      {
         crypted += grid[(X.first + 1) % dim][X.second];
         crypted += grid[(A.first + 1) % dim][A.second];
      }
         // R�gle 4 : Si x = a ET y = b, alors X = (x,y) et A = NUL = X.
      else
      {
         crypted += grid[X.first][X.second];
         crypted += "X";
      }
   }

   return crypted;
}

// Encode un texte clair avec la m�thode de Playfair.

const Playfair::ClassicalType Playfair::decode(const ClassicalType &cipher_text)
{
   unsigned int cipher_len = cipher_text.length();
   ClassicalType decrypted = "";
   decrypted.reserve(cipher_len);
   
   const Grid grid(getGrid(key + alpha));

   for (unsigned int i = 0; i < cipher_len; i += 2)
   {
      // Obtenir les coordonnées (x,y) et (a,b) des lettres du bigramme dans la grille.
      const auto x = getCharCoordinates(cipher_text[i], grid);
      const auto y = getCharCoordinates(cipher_text[i+1], grid);

      // Soient A = (x,y) et B = (a,b)
      // R�gle 1 : Si x != a ET y != b, alors A = (x,b) et B = (a,y).
      if (x.first != y.first && x.second != y.second)
      {
         decrypted += grid[x.first][y.second];
         decrypted += grid[y.first][x.second];
      }
         // R�gle 2 : Si x = a ET y != b, alors A = (x+1,y) et B = (a+1,b).
      else if (x.first == y.first && x.second != y.second)
      {
         decrypted += grid[x.first][(x.second + dim - 1) % dim];
         decrypted += grid[y.first][(y.second + dim - 1) % dim];
      }
         // R�gle 3 : Si x != a ET y = b, alors A = (x,y+1) et B = (a,b+1).
      else if (x.first != y.first && x.second == y.second)
      {
         decrypted += grid[(x.first + dim - 1) % dim][x.second];
         decrypted += grid[(y.first + dim - 1) % dim][y.second];
      }
   }

   return decrypted;
}