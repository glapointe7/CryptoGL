#include "Playfair.h"
#include "Data.h"
#include "Tools.h"
#include <utility>
#include <iostream>

Playfair::Playfair()
{
   clear_text = Data::load("clear_text.txt");
   cipher_text = Data::load("cipher_text.txt");
   clear_len = clear_text.length();
   cipher_len = cipher_text.length();
   setAlpha("ABCDEFGHIJKLMNOPQRSTUVXYZ");
   if ((clear_text.length() % 2) != 0)
   {
      clear_text += "X";
      clear_len++;
   }
}

Playfair::~Playfair()
{
}

void Playfair::setAlpha(const string letters)
{
   alpha = letters;
}

void Playfair::setKey(const string key)
{
   this->key = key;
}

// Encode un texte clair avec la méthode de Playfair.

string Playfair::encode() const
{
   string crypted = "";
   crypted.reserve(clear_len);

   const string str = key + alpha;
   vector<string> grid(createGrid(str, rows));

   for (unsigned int i = 0; i < clear_len; i += 2)
   {
      // Obtenir les coordonnées (x,y) et (a,b) des lettres du bigramme dans la grille.
      auto X = make_pair(0, 0);
      auto A = make_pair(0, 0);
      unsigned int y = 0, b = 0;

      for (auto row : grid)
      {
         X.first = row.find(clear_text[i]);
         if (X.first != string::npos)
         {
            X.second = y;
            break;
         }
         y++;
      }

      for (auto row : grid)
      {
         A.first = row.find(clear_text[i + 1]);
         if (A.first != string::npos)
         {
            A.second = b;
            break;
         }
         b++;
      }
      cout << clear_text[i] << "(" << X.first << ", "
              << X.second << ") - " << clear_text[i + 1] << "("
              << A.first << ", "
              << A.second << ")";

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
         crypted += grid[X.first][(X.second + 1) % 5];
         crypted += grid[A.first][(A.second + 1) % 5];
      }
         // R�gle 3 : Si x != a ET y = b, alors X = (x,y+1) et A = (a,b+1).
      else if (X.first != A.first && A.second == X.second)
      {
         crypted += grid[(X.first + 1) % 5][X.second];
         crypted += grid[(A.first + 1) % 5][A.second];
      }
         // R�gle 4 : Si x = a ET y = b, alors X = (x,y) et A = NUL = X.
      else
      {
         crypted += grid[X.first][X.second];
         crypted += "X";
      }
      cout << " => " << crypted[i] << crypted[i + 1] << "\n";
   }

   Data::save("cipher_text.txt", crypted);

   return crypted;
}

// Encode un texte clair avec la m�thode de Playfair.

string Playfair::decode() const
{
   string decrypted = "";
   decrypted.reserve(cipher_len);
   vector<string> grid(createGrid(key + alpha, Playfair::rows));

   for (unsigned int i = 0; i < cipher_len; i += 2)
   {
      // Obtenir les coordonn�es (x,y) des lettres du bigramme dans la grille.
      auto x = make_pair(0, 0);
      auto y = make_pair(0, 0);
      unsigned int j = 0;
      for (auto row : grid)
      {
         x = make_pair(row.find(clear_text[i]), row.find(clear_text[i + 1]));
         if (x.first != string::npos)
         {
            y.first = j;
         }
         if (x.second != string::npos)
         {
            y.second = j;
         }
         j++;
      }

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
         decrypted += grid[x.first][(x.second + 4) % 5];
         decrypted += grid[y.first][(y.second + 4) % 5];
      }
         // R�gle 3 : Si x != a ET y = b, alors A = (x,y+1) et B = (a,b+1).
      else if (x.first != y.first && x.second == y.second)
      {
         decrypted += grid[(x.first + 4) % 5][x.second];
         decrypted += grid[(y.first + 4) % 5][y.second];
      }
   }

   Data::save("clear_text.txt", decrypted);

   return decrypted;
}