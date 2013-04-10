
#ifndef FLEISSNER_H
#define FLEISSNER_H

#include "StringCipher.h"

#include <vector>
#include <string>

class Fleissner : public StringCipher
{  
public:
   
   typedef std::pair<unsigned short, unsigned short> Coordinates;
   
   Fleissner();

   virtual std::string encode(const std::string &);
   virtual std::string decode(const std::string &);

   void setKey(const std::vector<Coordinates> key);
   void setGridDimension(const unsigned short dim);

private:
   bool checkMask(std::vector<Coordinates> &coords) const;
   void fillWithRandomChars(std::string &text);

   // Dimension de la grille de chiffrement.
   unsigned short grid_dim;

   // Coordonnées des masques initial, rotation 90, rotation 180 et rotation 270.
   //vector<coordinates> north;
   //vector<coordinates> east;
   //vector<coordinates> south;
   //vector<coordinates> west;

   //vector<coordinates> coords;
   // Coordonnées des masques initial.
   std::vector<Coordinates> key;
};

#endif // FLEISSNER_H