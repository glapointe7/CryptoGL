
#include "Adfgvx.h"

#include <utility>
#include <ctime>
#include <map>

#include "Tools.h"

Adfgvx::Adfgvx()
{
   setAlpha("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
}

void Adfgvx::setGridKey(const std::vector<std::string> &grid)
{
   grid_key = grid;
}

std::string Adfgvx::encode(const std::string &clear_text)
{
   std::string key_ok = removeRepeatedLetters(key);
   std::string crypted = "";
   unsigned int clear_len = clear_text.length();
   unsigned int key_len = key_ok.length();
   crypted.reserve((clear_len << 1) + key_len);

   // Étape 1 : On prend les coordonnées de chaque lettre et on les remplace
   // par A,D,F,G,V ou X tels que A=0, D=1, F=2, G=3, V=4, X=5.
   // Exemple : Si la lettre 'K' se situe à (2,3), alors K s'encode FG.
   std::string first_encoding(crypted);
   for (auto c : clear_text)
   {
      auto coords = getCharCoordinates(c, grid_key);

      first_encoding += code[coords.second];
      first_encoding += code[coords.first];
   }

   // Étape 2 : On surchiffre à l'aide d'une transposition avec la clé key.
   // On veut donc obtenir une première grille selon la clé.

   // On doit avoir un multiple de la longueur de la clé. Le reste sera rempli par
   // A,D,F,G,V,X au hasard.
   unsigned int rest = clear_len % key_len;
   srand(time(0));
   for (unsigned int i = 0; i < rest; i++)
   {
      first_encoding += code[rand() % dim];
   }

   // On remplit la grille qu'on doit lire en colonne. 
   // Pour faciliter le code, on la transpose.
   std::vector<std::string> grid;
   unsigned int first_encoding_len = first_encoding.length();
   for (unsigned int i = 0; i < key_len; i++)
   {
      std::string tmp = "";
      tmp.reserve(first_encoding_len);
      for (unsigned int k = i; k < first_encoding_len; k += key_len)
      {
         tmp += first_encoding[k];
      }
      grid.push_back(tmp);
   }
   
   // Étape 3 : On permute les lignes selon l'ordre alphabétique des lettres de la clé.
   // On crée un map [char=>integer] qui est ordonné où char est une lettre
   // de la clé.
   std::map<char, unsigned int> sorted_key;
   for(unsigned int i = 0; i < key_len; i++)
   {
      sorted_key.insert(make_pair(key_ok[i], i));
   }
   
   for(auto pair : sorted_key)
   {
      crypted += grid[pair.second];
   }

   return crypted;
}

std::string Adfgvx::decode(const std::string &cipher_text)
{
   std::string decrypted = "";
   //decrypted.reserve(cipher_text >> 1);
   
   std::string key_ok = removeRepeatedLetters(key);
   unsigned int key_len = key_ok.length();
   
   // Étape 1 : On construit la grille chiffrée selon la clé.
   std::map<char, unsigned int> sorted_key;
   for(unsigned int i = 0; i < key_len; i++)
   {
      sorted_key.insert(make_pair(key_ok[i], i));
   }
   
   std::vector<std::string> grid;
   for(auto c : key_ok)
   {
      grid.push_back(cipher_text.substr(sorted_key[c] * key_len, key_len));
   }
   
   // On prend les caractères de la grille transposée.
   unsigned int cipher_len = cipher_text.length();
   std::string first_decoding = "";
   first_decoding.reserve(cipher_len);
   for (unsigned int i = 0; i < key_len; i++)
   {
      for (auto str : grid)
      {
         first_decoding += str[i];
      }
   }
   
   for(unsigned int i = 0; i < cipher_len; i += 2)
   {
      auto coords = make_pair(code.find(first_decoding[i]), code.find(first_decoding[i+1]));
      decrypted += grid_key[coords.first][coords.second];
   }
   
   return decrypted;
}