#ifndef FLEISSNER_H
#define FLEISSNER_H

#include "StringCipher.h"
#include <vector>

typedef pair<unsigned short, unsigned short> coordinates;

class Fleissner : public StringCipher
{
public:
   Fleissner();
   ~Fleissner();

   string encode();
   string decode();

   void setAlpha(const string letters);
   void setKey(const vector<coordinates> key);
   void setGridDimension(const unsigned short dim);

private:
   bool checkMask(vector<coordinates> &coords) const;
   void fillWithRandomChars();

   // Dimension de la grille de chiffrement.
   unsigned short grid_dim;

   // Coordonnées des masques initial, rotation 90, rotation 180 et rotation 270.
   //vector<coordinates> north;
   //vector<coordinates> east;
   //vector<coordinates> south;
   //vector<coordinates> west;

   //vector<coordinates> coords;
   // Coordonnées des masques initial.
   vector<coordinates> key;

   unsigned int cipher_len;
   unsigned int clear_len;
};
#endif