
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

   virtual const ClassicalType encode(const ClassicalType &);
   virtual const ClassicalType decode(const ClassicalType &);

   void setKey(const std::vector<Coordinates> key);
   void setGridDimension(const unsigned short dim);

private:
   bool checkMask(std::vector<Coordinates> &coords) const;
   void fillWithRandomChars(ClassicalType &text);

   // Dimension de la grille de chiffrement.
   unsigned short grid_dim;

   std::vector<Coordinates> key;
};

#endif