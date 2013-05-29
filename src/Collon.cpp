
#include "Collon.hpp"

// Series length have to be between 1 and text length.

void Collon::setBlockLength(const uint32_t block_len)
{
   if (block_len == 0)
   {
      throw ZeroBlockLength("The block length you provided has to be greater than zero.");
   }
   
   this->block_len = block_len;
}

const Collon::ClassicalType Collon::encode(const ClassicalType &clear_text)
{
   const uint32_t clear_len = clear_text.length();
   std::string line1, line2;
   line1.reserve(clear_len);
   line2.reserve(clear_len);
   ClassicalType crypted;
   crypted.reserve(clear_len * 2);

   // Création de la grille de chiffrement avec lettres doublons effacées.
   const Grid grid(getGrid(getKey() + alpha));

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
   //const uint32_t rest = clear_len % block_len;
   //const uint32_t clear_rest = clear_len - rest;
   for (uint32_t j = 0; j < clear_len; j += block_len)
   {
      crypted += line1.substr(j, block_len);
      crypted += line2.substr(j, block_len);
   }
   //crypted += line1.substr(clear_rest);
   //crypted += line2.substr(clear_rest);

   return crypted;
}

const Collon::ClassicalType Collon::decode(const ClassicalType &cipher_text)
{
   const uint32_t cipher_len = cipher_text.length();
   const uint32_t line_len = cipher_len / 2;

   ClassicalType decrypted;
   decrypted.reserve(line_len);
   std::string line1, line2;
   line1.reserve(line_len);
   line2.reserve(line_len);

   const Grid grid(getGrid(getKey() + alpha));

   // Remise des bigrammes en 2 lignes de texte. Généralement, la longueur de la ligne n'est
   // pas multiple de la longueur du bloc. Pour cela, on doit garder le reste.
   const uint32_t double_block_len = block_len << 1;
   const uint32_t line_rest_len = line_len % double_block_len;
   const uint32_t line_blocks_len = cipher_len - (line_rest_len << 1);

   // Obtention des 2 lignes sans reste puis on ajoute le reste.
   for (uint32_t i = 0; i < line_blocks_len; i += double_block_len)
   {
      line1 += cipher_text.substr(i, block_len);
      line2 += cipher_text.substr(i + block_len, block_len);
   }
   line1 += cipher_text.substr(line_blocks_len, line_rest_len);
   line2 += cipher_text.substr(line_blocks_len + line_rest_len, line_rest_len);

   // Soient A = (x1, y1) et B = (x2, y2) les 2 lettres du bigramme à l'itération i.
   // Soit C = (x, y) la lettre décodée. On doit avoir C = (x2, y1).
   for (uint32_t i = 0; i < line_len; i++)
   {
      const auto A = getCharCoordinates(line1[i], grid);
      const auto B = getCharCoordinates(line2[i], grid);

      decrypted += grid[A.second][B.first];
   }

   return decrypted;
}