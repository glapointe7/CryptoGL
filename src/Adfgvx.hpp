
#ifndef ADFGVX_HPP
#define	ADFGVX_HPP

#include "SquareCipher.hpp"
#include <vector>
#include <string>

class Adfgvx : public SquareCipher
{
public:
   Adfgvx();

   const ClassicalType encode(const ClassicalType &) final;
   const ClassicalType decode(const ClassicalType &) final;

   void setGridKey(const Grid &grid);

private:
   // Grille 6X6 de caractères A-Z, 0-9 servant de 2e clé.
   Grid grid_key;

   // Dimension de la grille carrée.
   const std::string code = "ADFGVX";
};

#endif	/* ADFGVX_H */
