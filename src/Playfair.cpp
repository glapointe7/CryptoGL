
#include "Playfair.hpp"

#include "exceptions/EmptyKey.hpp"
#include "exceptions/BadAlphaLength.hpp"

#include "MathematicalTools.hpp"

#include <utility>

// Encode un texte clair avec la méthode de Playfair.

const Playfair::ClassicalType Playfair::encode(const ClassicalType &clear_text)
{
   if(key.empty())
   {
      throw EmptyKey("You have to set the key before encoding your message.");
   }
   
   if(!isPerfectSquare(alpha.length()))
   {
      throw BadAlphaLength("The length of your alphabet should be a perfect square.", alpha.length());
   }
   
   ClassicalType full_text(appendChars(clear_text, 2, 'X'));
   unsigned int clear_len = full_text.length();
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
         crypted += grid[X.second][A.first];
         crypted += grid[A.second][X.first];
      }
         // R�gle 2 : Si x = a ET y != b, alors X = (x+1,y) et A = (a+1,b).
      else if (X.first == A.first && X.second != A.second)
      {
         crypted += grid[(X.second + 1) % dim][X.first];
         crypted += grid[(A.second + 1) % dim][A.first];
      }
         // R�gle 3 : Si x != a ET y = b, alors X = (x,y+1) et A = (a,b+1).
      else if (X.first != A.first && A.second == X.second)
      {
         crypted += grid[X.second][(X.first + 1) % dim];
         crypted += grid[A.second][(A.first + 1) % dim];
      }
         // R�gle 4 : Si x = a ET y = b, alors X = (x,y) et A = NUL = X.
      else
      {
         full_text.insert(full_text.begin() + i + 1, 'X');
         i -= 2;
         clear_len++;
      }
   }

   return crypted;
}

// Encode un texte clair avec la m�thode de Playfair.

const Playfair::ClassicalType Playfair::decode(const ClassicalType &cipher_text)
{
   if(key.empty())
   {
      throw EmptyKey("You have to set the key before decoding your message.");
   }
   
   if(!isPerfectSquare(alpha.size()))
   {
      throw BadAlphaLength("The length of your alphabet should be a perfect square.", alpha.length());
   }
   
   const unsigned int cipher_len = cipher_text.length();
   ClassicalType decrypted = "";
   decrypted.reserve(cipher_len);
   
   const Grid grid(getGrid(key + alpha));

   for (unsigned int i = 0; i < cipher_len; i += 2)
   {
      // Obtenir les coordonnées (x,y) et (a,b) des lettres du bigramme dans la grille.
      const auto X = getCharCoordinates(cipher_text[i], grid);
      const auto A = getCharCoordinates(cipher_text[i+1], grid);

      // Soient A = (x,y) et B = (a,b)
      // R�gle 1 : Si x != a ET y != b, alors A = (x,b) et B = (a,y).
      if (X.first != A.first && X.second != A.second)
      {
         decrypted += grid[X.second][A.first];
         decrypted += grid[A.second][X.first];
      }
         // R�gle 2 : Si x = a ET y != b, alors A = (x+1,y) et B = (a+1,b).
      else if (X.first == A.first && X.second != A.second)
      {
         decrypted += grid[(X.second + dim - 1) % dim][X.first];
         decrypted += grid[(A.second + dim - 1) % dim][A.first];
      }
         // R�gle 3 : Si x != a ET y = b, alors A = (x,y+1) et B = (a,b+1).
      else if (X.first != A.first && X.second == A.second)
      {
         decrypted += grid[X.second][(X.first + dim - 1) % dim];
         decrypted += grid[A.second][(A.first + dim - 1) % dim];
      }
   }

   return decrypted;
}