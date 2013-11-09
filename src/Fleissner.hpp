
#ifndef FLEISSNER_HPP
#define FLEISSNER_HPP

#include "StringCipher.hpp"

#include "exceptions/BadKey.hpp"

#include <vector>
#include <string>
#include <set>

class Fleissner : public StringCipher
{  
   using Cell = std::pair<uint32_t, uint32_t>;
   using Coordinates = std::vector<Cell>;
   using KeyCellNotUnique = BadKey;
   
public:
   Fleissner(const Coordinates &key, const uint32_t grid_dim, const bool clockwise);
   
   virtual const ClassicalType encode(const ClassicalType &clear_text) final;
   virtual const ClassicalType decode(const ClassicalType &cipher_text) final;

   void setGridDimension(const uint32_t dim);

private:   
   void rotationExists(std::set<Cell> &cmp, const uint32_t x, const uint32_t y) const;

   // Dimension of the cipher grid.
   uint32_t grid_dim;

   Coordinates key;
};

#endif