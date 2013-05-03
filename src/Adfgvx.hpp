
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
   static const uint8_t is6X6(const Grid &grid);
   
   // Grille 6X6 de caractères A-Z, 0-9 servant de 2e clé.
   Grid grid_key;

   // Dimension de la grille carrée.
   static const std::string code;
};

#endif