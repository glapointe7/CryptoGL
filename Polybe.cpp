/* Modifié par Gabriel Lapointe
 * le 27 mars 2013
 */

#include "Polybe.h"
#include "Data.h"
#include "Tools.h"
#include <utility> 

Polybe::Polybe()
{
   clear_text = Data::load("clear_text.txt");
   cipher_text = Data::load("cipher_text.txt");
   clear_len = clear_text.length();
   cipher_len = cipher_text.length();
   setAlpha("ABCDEFGHIJKLMNOPQRSTUVXYZ");
}

Polybe::~Polybe()
{
}

void Polybe::setAlpha(const string letters)
{
   alpha = letters;
}

void Polybe::setKey(const string key)
{
   this->key = key;
}

// Encode un texte clair par le chiffre de Polybe.

string Polybe::encode()
{
   string crypted = "";
   crypted.reserve(2 * clear_len);

   // Construction de la grille de chiffrement.
   vector<string> grid(createGrid(key + alpha, Polybe::rows));

   for (auto c : clear_text)
   {
      auto pos = make_pair('1', '1');
      for (auto row : grid)
      {
         pos.first = row.find(c);
         if (pos.first != string::npos)
         {
            pos.first += '1';
            break;
         }
         pos.second++;
      }

      crypted += pos.second;
      crypted += pos.first;
   }

   Data::save("cipher_text.txt", crypted);

   return crypted;
}

// Décode un texte encodé par le chiffre de Polybe.

string Polybe::decode()
{
   string decrypted = "";
   decrypted.reserve(clear_len);
   vector<string> grid(createGrid(key + alpha, Polybe::rows));

   for (unsigned i = 0; i < cipher_len; i += 2)
   {
      auto pos = make_pair(cipher_text[i] - '1', cipher_text[i + 1] - '1');
      decrypted += grid[pos.first][pos.second];
   }

   Data::save("clear_text.txt", decrypted);

   return decrypted;
}