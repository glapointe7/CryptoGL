
#include "Collon.hpp"
#include "exceptions/BadAlphaLength.hpp"
#include "MathematicalTools.hpp"
#include "exceptions/BadGridDimension.hpp"
#include "exceptions/EmptyKey.hpp"

// Series length have to be between 1 and text length.

void Collon::setBlockLength(const unsigned int series)
{
   block_len = series;
}

const Collon::ClassicalType Collon::encode(const ClassicalType &clear_text)
{
   const unsigned int clear_len = clear_text.length();
   if(block_len <= 0 || block_len > clear_len)
   {
      throw Exception("The block length you specified should be between 1 and the length of your message.");
   }
   
   if(key.empty())
   {
      throw EmptyKey("You have to set the key before encoding your message.");
   }
   
   if(!isPerfectSquare(alpha.length()))
   {
      throw BadAlphaLength("The length of your alphabet should be a perfect square.", alpha.length());
   }
   
   std::string line1 = "";
   line1.reserve(clear_len);
   std::string line2(line1);
   ClassicalType crypted = "";
   crypted.reserve(clear_len << 1);

   // Création de la grille de chiffrement avec lettres doublons effacées.
   const Grid grid(getGrid(key + alpha));

   // Chaque caractères situé en (x,y) est encodé par un bigramme AB tel que
   // A = (a,y) et B = (x,b).
   for (const auto c : clear_text)
   {
      const auto coords = getCharCoordinates(c, grid);

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

const Collon::ClassicalType Collon::decode(const ClassicalType &cipher_text)
{
   const unsigned int cipher_len = cipher_text.length();
   const unsigned int line_len = cipher_len >> 1;
   if(block_len <= 0 || block_len > line_len)
   {
      throw Exception("The block length you specified should be between 1 and the half length of your message.");
   }
   
   if(key.empty())
   {
      throw EmptyKey("You have to set the key before decoding your message.");
   }
   
   if(!isPerfectSquare(alpha.length()))
   {
      throw BadAlphaLength("The length of your alphabet should be a perfect square.", alpha.length());
   }
   
   ClassicalType decrypted = "";
   decrypted.reserve(line_len);
   std::string line1 = "";
   line1.reserve(line_len);
   std::string line2(line1);

   const Grid grid(getGrid(key + alpha));

   // Remise des bigrammes en 2 lignes de texte. Généralement, la longueur de la ligne n'est
   // pas multiple de la longueur du bloc. Pour cela, on doit garder le reste.
   const unsigned short double_block_len = block_len << 1;
   const unsigned int line_rest_len = line_len % double_block_len;
   const unsigned int line_blocks_len = cipher_len - (line_rest_len << 1);

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
      const auto A = getCharCoordinates(line1[i], grid);
      const auto B = getCharCoordinates(line2[i], grid);

      decrypted += grid[A.second][B.first];
   }

   return decrypted;
}