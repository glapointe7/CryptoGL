
#ifndef ADFGVX_HPP
#define	ADFGVX_HPP

#include "SquareCipher.hpp"
#include <vector>
#include <string>

class Adfgvx : public SquareCipher
{
public:
   Adfgvx(const KeyType &key);

   virtual const ClassicalType encode(const ClassicalType &clear_text) final;
   virtual const ClassicalType decode(const ClassicalType &cipher_text) final;

   void setGridKey(const Grid &grid);

private:
   static uint8_t is6X6(const Grid &grid);
   const std::vector<int32_t> getPermutationKey() const;
   
   // Grille 6X6 de caractères A-Z, 0-9 servant de 2e clé.
   Grid grid_key;

   static const std::string code;
};

#endif