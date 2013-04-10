
#ifndef ADFGVX_H
#define	ADFGVX_H

#include "SquareCipher.h"
#include <vector>
#include <string>

class Adfgvx : public SquareCipher
{
public:
   Adfgvx();

   virtual std::string encode(const std::string &);
   virtual std::string decode(const std::string &);

   void setGridKey(const std::vector<std::string> &grid);

private:
   // Grille 6X6 de caractères A-Z, 0-9 servant de 2e clé.
   std::vector<std::string> grid_key;

   // Dimension de la grille carrée.
   const std::string code = "ADFGVX";
};

#endif	/* ADFGVX_H */