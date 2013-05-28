
#ifndef FLEISSNER_HPP
#define FLEISSNER_HPP

#include "StringCipher.hpp"

#include "exceptions/BadKey.hpp"

#include <vector>
#include <string>
#include <set>

class Fleissner : public StringCipher
{  
   typedef std::pair<uint16_t, uint16_t> Cell;
   typedef std::vector<Cell> Coordinates;
   using KeyCellNotUnique = BadKey;
   
public:
   Fleissner(const Coordinates &key, const uint16_t grid_dim, const bool clockwise);
   
   virtual const ClassicalType encode(const ClassicalType &clear_text) final;
   virtual const ClassicalType decode(const ClassicalType &cipher_text) final;

   void setGridDimension(const uint16_t dim);

private:   
   void rotationExists(std::set<Cell> &cmp, const uint16_t x, const uint16_t y) const;

   // Dimension of the cipher grid.
   uint16_t grid_dim;

   Coordinates key;
};

#endif