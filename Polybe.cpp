
#include "Polybe.h"

#include <utility> 

using namespace std;

Polybe::Polybe()
{
   setAlpha("ABCDEFGHIJKLMNOPQRSTUVXYZ");
}

// Encode un texte clair par le chiffre de Polybe.

string Polybe::encode(const std::string &clear_text)
{
   unsigned int clear_len = clear_text.length();
   string crypted = "";
   crypted.reserve(clear_len << 1);

   // Construction de la grille de chiffrement.
   vector<string> grid(getGrid(key + alpha));

   // Obtention des coordonnées de chaque lettre dans la grille.
   // Note : on ajoute '1', car on veut rendre le cryptogramme en string.
   for (auto c : clear_text)
   {
      auto pos = getCharCoordinates(c, grid);

      crypted += (pos.second + '1');
      crypted += (pos.first + '1');
   }

   return crypted;
}

// Décode un texte encodé par le chiffre de Polybe.

string Polybe::decode(const std::string &cipher_text)
{
   unsigned int cipher_len = cipher_text.length();
   string decrypted = "";
   decrypted.reserve(cipher_len);
   vector<string> grid(getGrid(key + alpha));

   for (unsigned i = 0; i < cipher_len; i += 2)
   {
      auto pos = make_pair(cipher_text[i] - '1', cipher_text[i + 1] - '1');
      decrypted += grid[pos.first][pos.second];
   }

   return decrypted;
}