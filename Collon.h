/* 
 * File:   Collon.h
 * Author: gabriel
 *
 * Created on March 29, 2013, 8:54 AM
 */

#ifndef COLLON_H
#define	COLLON_H

#include "StringCipher.h"

class Collon : public StringCipher
{
public:
   Collon();
   Collon(const Collon& orig);
   ~Collon();

   string encode() const;
   string decode() const;

   void setKey(const string key);
   void setGridDimension(const uint8_t grid_dim);
   void setBlockLength(const uint32_t series);
   void setAlpha(const string letters);

private:
   string key;
   uint32_t clear_len;
   uint32_t cipher_len;

   // Dimension de la grille carrée.
   uint8_t dim;

   // Longueur des séries (bloc de caractères).
   uint32_t block_len;
};

#endif	/* COLLON_H */

