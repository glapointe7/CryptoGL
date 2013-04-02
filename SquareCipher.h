#ifndef SQUARECIPHER_H
#define	SQUARECIPHER_H

#include "StringCipher.h"

#include <string>
#include <utility>
#include <vector>

typedef std::pair<unsigned char, unsigned char> Coordinates;

class SquareCipher : public StringCipher
{
public:
   SquareCipher();
   virtual ~SquareCipher() {}

   virtual std::string encode(const std::string &) = 0;
   virtual std::string decode(const std::string &) = 0;

   void setGridDimension(const unsigned char dim);
   void setKey(const std::string &key);

protected:
   std::vector<std::string> getGrid(const std::string &chars) const;
   Coordinates getCharCoordinates(const char c, const std::vector<std::string> &grid) const;

   std::string key;

   // Grille de chiffrement.
   //std::vector<std::string> grid;

   // Dimension de la grille carrée.
   unsigned char dim;
};

#endif	/* SQUARECIPHER_H */

