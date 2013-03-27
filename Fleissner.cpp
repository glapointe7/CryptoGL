#include "Fleissner.h"
#include "Data.h"
#include <set>
#include <time.h>

Fleissner::Fleissner()
{
   clear_text = Data::load("clear_text.txt");
   cipher_text = Data::load("cipher_text.txt");
   clear_len = clear_text.length();
   cipher_len = cipher_text.length();
   setAlpha("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.!,:;?_-'\"/*+=()[]{}%$#");
}

Fleissner::~Fleissner()
{
}

void Fleissner::setAlpha(const string letters)
{
   alpha = letters;
}

// La clé correspond au masque, c'est-à-dire les coordonnées de chaque trous du masque.

void Fleissner::setKey(const vector<coordinates> key)
{
   this->key = key;
}

void Fleissner::setGridDimension(const unsigned short dim)
{
   grid_dim = dim;
}

// V�rifie que le masque initial contient exactement le quart des cellules de la grille.

bool Fleissner::checkMask(vector<coordinates> &coords) const
{
   unsigned short key_size = key.size();
   unsigned short mask_size_approuved = key_size * 4;

   // La matrice doit être carrée.
   if (mask_size_approuved != (grid_dim * grid_dim))
   {
      return false;
   }

   // Afin d'ordonner les coordonnées pour chaque rotation.
   set<coordinates> rotation90, rotation180, rotation270;
   set<coordinates> cmp;
   pair < set<coordinates>::iterator, bool> is_unique;

   // On remplit le SET du masque initial key et on insert les rotations ordonnées 
   // en s'assurant que les coordonnées sont uniques.
   for (auto xy : key)
   {
      cmp.insert(make_pair(xy.first, xy.second));
   }

   // On vérifie si les coordonnées des rotations existent. 
   // Si l'une d'elles existe, alors le masque initial n'est pas valide.
   for (auto xy : key)
   {
      // 270 degr�s
      is_unique = cmp.insert(make_pair(grid_dim - 1 - xy.second, xy.first));
      if (is_unique.second == false)
      {
         return false;
      }
      rotation270.insert(make_pair(grid_dim - 1 - xy.second, xy.first));
   }
   coords.insert(coords.end(), rotation270.begin(), rotation270.end());

   for (auto xy : key)
   {
      // 180 degr�s
      is_unique = cmp.insert(make_pair(grid_dim - 1 - xy.first, grid_dim - 1 - xy.second));
      if (is_unique.second == false)
      {
         return false;
      }
      rotation180.insert(make_pair(grid_dim - 1 - xy.first, grid_dim - 1 - xy.second));
   }
   coords.insert(coords.end(), rotation180.begin(), rotation180.end());

   for (auto xy : key)
   {
      // 90 degr�s
      is_unique = cmp.insert(make_pair(xy.second, grid_dim - 1 - xy.first));
      if (is_unique.second == false)
      {
         return false;
      }
      rotation90.insert(make_pair(xy.second, grid_dim - 1 - xy.first));
   }
   coords.insert(coords.end(), rotation90.begin(), rotation90.end());

   return true;
}

// Remplit le texte clair par des caract�res au hasard afin d'obtenir un multiple du carr� de grid_dim.

void Fleissner::fillWithRandomChars()
{
   unsigned int fillers = clear_len % (grid_dim * grid_dim);
   unsigned short alpha_len = alpha.length();
   srand(time(0));

   for (unsigned int i = 0; i < fillers; i++)
   {
      unsigned short rnd_alpha_pos = rand() % alpha_len + 1;
      clear_text += alpha[rnd_alpha_pos];
   }
   clear_len += fillers;
}

// Encode un texte avec la grille tournante de Fleissner.

string Fleissner::encode() const
{
   string crypted = "";
   vector<coordinates> coords(key);
   unsigned short dim = grid_dim * grid_dim;

   // Si le masque est valide.
   if (checkMask(coords) == true)
   {
      fillWithRandomChars();

      // On réserve l'espace selon grid_dim pour la grille.
      vector<string> grid;
      grid.reserve(grid_dim);
      for (unsigned short p = 0; p < grid_dim; p++)
      {
         grid[p].reserve(grid_dim);
      }

      // Construire la chaîne de cryptage complète.
      // Si la grille est pleine, alors on la vide et on recommence le processus.
      unsigned short max_grid = static_cast<unsigned short> (clear_len / dim);
      unsigned int k = 0;

      for (unsigned short i = 0; i < max_grid; i++)
      {
         // Remplir la grille de chiffrement selon les coordonnées recueillies.
         for (unsigned short j = 0; j < dim; j++, k++)
         {
            grid[coords[j].first][coords[j].second] = clear_text[k];
         }

         // Construire la cha�ne de chiffrement et vider la grille.
         for (auto str : grid)
         {
            crypted += str;
         }
         grid.clear();
      }

      //General::save(crypted);
   }

   return crypted;
}

// Décode un cryptogramme de la grille de Fleissner.

string Fleissner::decode() const
{
   string decrypted = "";
   vector<coordinates> coords(key);
   unsigned short dim = grid_dim * grid_dim;

   // On v�rifie si le masque initial est valide et on obtient les coordonn�es de ses rotations.
   if (checkMask(coords) == true)
   {
      unsigned short max_grid = static_cast<unsigned short> (cipher_len / dim);
      unsigned int k = 0;
      vector<string> grid;

      for (unsigned int i = 0; i < max_grid; i++)
      {
         // On remplit la grille du cipher text.
         for (unsigned short j = 0; j < grid_dim; j++)
         {
            grid.push_back(cipher_text.substr(k, grid_dim));
            k += grid_dim;
         }

         for (unsigned int c = 0; c < dim; c++)
         {
            decrypted += grid[coords[c].first][coords[c].second];
         }
      }

      //General::save(decrypted);
   }

   return decrypted;
}