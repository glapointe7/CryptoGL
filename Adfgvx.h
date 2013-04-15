
#ifndef ADFGVX_H
#define	ADFGVX_H

#include "SquareCipher.h"
#include <vector>
#include <string>

class Adfgvx : public SquareCipher
{
public:
   Adfgvx();

   virtual const String encode(const String &);
   virtual const String decode(const String &);

   void setGridKey(const Grid &grid);

private:
   // Grille 6X6 de caractères A-Z, 0-9 servant de 2e clé.
   Grid grid_key;

   // Dimension de la grille carrée.
   const std::string code = "ADFGVX";
};

#endif	/* ADFGVX_H */
