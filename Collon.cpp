/* 
 * File:   Collon.cpp
 * Author: gabriel
 * 
 * Created on March 29, 2013, 8:54 AM
 */

#include "Collon.h"
#include "Data.h"
#include "Tools.h"
#include <utility>

Collon::Collon()
{
   clear_text = Data::load("clear_text.txt");
   cipher_text = Data::load("cipher_text.txt");
   clear_len = clear_text.length();
   cipher_len = cipher_text.length();
   setAlpha("ABCDEFGHIJKLMNOPQRSTUVXYZ");
}

Collon::Collon(const Collon& orig)
{
}

Collon::~Collon()
{
}

void Collon::setKey(const string key)
{
   this->key = key;
}

void Collon::setGridDimension(const uint8_t grid_dim)
{
   dim = grid_dim;
}

void Collon::setBlockLength(const uint32_t series)
{
   block_len = series;
}

void Collon::setAlpha(const string letters)
{
   alpha = letters;
}

string Collon::encode()
{
   string line1 = "";
   line1.reserve(clear_len);
   string line2(line1);
   string crypted = "";
   crypted.reserve(clear_len << 1);

   // Création de la grille de chiffrement avec lettres doublons effacées.
   vector<string> grid(createGrid(key + alpha, dim));

   // Chaque caractères situé en (x,y) est encodé par un bigramme AB tel que
   // A = (a,y) et B = (x,b).
   for (auto c : clear_text)
   {
      auto coordinate = make_pair(0, 0);
      for (auto str : grid)
      {
         coordinate.first = str.find(c);
         if (coordinate.first != string::npos)
         {
            break;
         }
         coordinate.second++;
      }

      line1 += grid[coordinate.second][0];
      line2 += grid[dim - 1][coordinate.first];
   }
   
   // On lit ensuite le texte par bloc. Par exemple pour un bloc de 3 :
   // L1 = MHD et L2 = JME => crypted = MHDJME. On recommence pour tout le texte.
   for (unsigned int j = 0; j < clear_len; j += block_len)
   {
      crypted += line1.substr(j, block_len);
      crypted += line2.substr(j, block_len);
   }
   //Data::save("cipher_text.txt", crypted);

   return crypted;
}

string Collon::decode()
{
   unsigned int line_len = cipher_len >> 1;
   string decrypted = "";
   decrypted.reserve(line_len);
   string line1 = "";
   line1.reserve(line_len);
   string line2(line1);
   
   vector<string> grid(createGrid(key + alpha, dim));
   
   // Remise des bigrammes en 2 lignes de texte. Généralement, la longueur de la ligne n'est
   // pas multiple de la longueur du bloc. Pour cela, on doit garder le reste.
   unsigned short double_block_len = block_len << 1;
   unsigned int line_rest_len = line_len % double_block_len;
   unsigned int line_blocks_len = cipher_len - (line_rest_len << 1);

   // Obtention des 2 lignes sans reste puis on ajoute le reste.
   for (unsigned int i = 0; i < line_blocks_len; i += double_block_len)
   {
      line1 += cipher_text.substr(i, block_len);
      line2 += cipher_text.substr(i + block_len, block_len);
   }
   line1 += cipher_text.substr(line_blocks_len, line_rest_len);
   line2 += cipher_text.substr(line_blocks_len + line_rest_len, line_rest_len);
  
   for (unsigned int i = 0; i < line_len; i++)
   {
      auto A = make_pair(0, 0);
      auto B = make_pair(0, 0);
      
      // Soient A = (x1, y1) et B = (x2, y2) les 2 lettres du bigramme à l'itération i.
      // Soit C = (x, y) la lettre décodée. On doit avoir C = (x2, y1).
      for(auto str : grid)
      {
         A.first = str.find(line1[i]);
         if(A.first != string::npos)
         {
            break;
         }
         A.second++;
      }
      
      for(auto str : grid)
      {
         B.first = str.find(line2[i]);
         if(B.first != string::npos)
         {
            break;
         }
         B.second++;
      }
      
      decrypted += grid[A.second][B.first];
   }

   return decrypted;
}