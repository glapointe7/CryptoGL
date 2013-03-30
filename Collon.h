
#ifndef COLLON_H
#define COLLON_H

#include "StringCipher.h"

class Collon : public StringCipher
{
public:
   Collon();

   std::string encode(const std::string &);
   std::string decode(const std::string &);

   void setKey(const std::string &key);
   void setGridDimension(const uint8_t grid_dim);
   void setBlockLength(const uint32_t series);

private:
   std::string key;
   uint32_t clear_len;
   uint32_t cipher_len;

   // Dimension de la grille carrée.
   uint8_t dim;

   // Longueur des séries (bloc de caractères).
   uint32_t block_len;
};

#endif // COLLON_H
