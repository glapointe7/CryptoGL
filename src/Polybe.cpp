
#include "Polybe.hpp"

#include <utility> 

Polybe::Polybe()
{
   setAlpha("ABCDEFGHIJKLMNOPQRSTUVXYZ");
}

// Encode un texte clair par le chiffre de Polybe.

const Polybe::ClassicalType Polybe::encode(const ClassicalType &clear_text)
{
   ClassicalType crypted = "";
   crypted.reserve(clear_text.length() << 1);

   // Construction de la grille de chiffrement.
   const Grid grid(getGrid(key + alpha));

   // Obtention des coordonnées de chaque lettre dans la grille.
   // Note : on ajoute '1', car on veut rendre le cryptogramme en string.
   for (auto c : clear_text)
   {
      const auto pos = getCharCoordinates(c, grid);

      crypted += (pos.second + '1');
      crypted += (pos.first + '1');
   }

   return crypted;
}

// Décode un texte encodé par le chiffre de Polybe.

const Polybe::ClassicalType Polybe::decode(const ClassicalType &cipher_text)
{
   const unsigned int cipher_len = cipher_text.length();
   ClassicalType decrypted = "";
   decrypted.reserve(cipher_len >> 1);
   const Grid grid(getGrid(key + alpha));

   for (unsigned i = 0; i < cipher_len; i += 2)
   {
      const auto pos = std::make_pair(cipher_text[i] - '1', cipher_text[i + 1] - '1');
      decrypted += grid[pos.first][pos.second];
   }

   return decrypted;
}