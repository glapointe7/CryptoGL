
#include "Collon.h"

#include <utility>

using namespace std;

Collon::Collon()
{
   setAlpha("ABCDEFGHIJKLMNOPQRSTUVXYZ");
}

void Collon::setBlockLength(const unsigned int series)
{
   block_len = series;
}

std::string Collon::encode(const string &clear_text)
{
   unsigned int clear_len = clear_text.length();
   string line1 = "";
   line1.reserve(clear_len);
   string line2(line1);
   string crypted = "";
   crypted.reserve(clear_len << 1);

   // Création de la grille de chiffrement avec lettres doublons effacées.
   vector<string> grid(getGrid(key + alpha));

   // Chaque caractères situé en (x,y) est encodé par un bigramme AB tel que
   // A = (a,y) et B = (x,b).
   for (auto c : clear_text)
   {
      auto coords = getCharCoordinates(c, grid);

      line1 += grid[coords.second][0];
      line2 += grid[dim - 1][coords.first];
   }

   // On lit ensuite le texte par bloc. Par exemple pour un bloc de 3 :
   // L1 = MHD et L2 = JME => crypted = MHDJME. On recommence pour tout le texte.
   for (unsigned int j = 0; j < clear_len; j += block_len)
   {
      crypted += line1.substr(j, block_len);
      crypted += line2.substr(j, block_len);
   }

   return crypted;
}

string Collon::decode(const string &cipher_text)
{
   unsigned int cipher_len = cipher_text.length();
   unsigned int line_len = cipher_len >> 1;
   string decrypted = "";
   decrypted.reserve(line_len);
   string line1 = "";
   line1.reserve(line_len);
   string line2(line1);

   vector<string> grid(getGrid(key + alpha));

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

   // Soient A = (x1, y1) et B = (x2, y2) les 2 lettres du bigramme à l'itération i.
   // Soit C = (x, y) la lettre décodée. On doit avoir C = (x2, y1).
   for (unsigned int i = 0; i < line_len; i++)
   {
      auto A = getCharCoordinates(line1[i], grid);
      auto B = getCharCoordinates(line2[i], grid);

      decrypted += grid[A.second][B.first];
   }

   return decrypted;
}