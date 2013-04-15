#ifndef SQUARECIPHER_H
#define	SQUARECIPHER_H

#include "StringCipher.h"

#include <string>
#include <utility>
#include <vector>

class SquareCipher : public StringCipher
{
public:
   typedef String Key;
   typedef std::pair<unsigned char, unsigned char> Coordinates;
   typedef std::vector<String> Grid;

   virtual const String encode(const String &) = 0;
   virtual const String decode(const String &) = 0;

   void setGridDimension(const unsigned int dim);
   void setKey(const Key &key);

protected:
   const Grid getGrid(const String &chars) const;
   static const Coordinates getCharCoordinates(const char c, const Grid &grid);

   Key key;

   // Grille de chiffrement.
   //std::vector<std::string> grid;

   // Dimension de la grille carrée.
   unsigned int dim;
};

#endif	/* SQUARECIPHER_H */

