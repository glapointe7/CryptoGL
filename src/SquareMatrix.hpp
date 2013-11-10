/*
 * Square matrix in M_k(Z_n).
 */
#ifndef SQUAREMATRIX_HPP
#define SQUAREMATRIX_HPP

#include "Types.hpp"

#include <vector>

class SquareMatrix
{
public:
   void setMatrix(const Int32Matrix &);
   Int32Matrix getMatrix() const;

   int32_t get(const uint32_t row, const uint32_t col) const;
   void set(const uint32_t row, const uint32_t col, const int32_t value);

   uint32_t getDimension() const { return dim; }
   void setDimension(const uint32_t);
   
   int32_t getModulo() const;
   void setModulo(const int32_t);

   /* Multiply a square matrix with a vector of the same dimension. */
   friend UInt32Vector operator *(const SquareMatrix *K, const UInt32Vector &V);

   Int32Matrix identity() const;
   
   int32_t det() const;
   SquareMatrix* inverse() const;
   
private:
   uint32_t findNonZero(const Int32Matrix &A, const uint32_t from) const;
   static bool isSquare(const Int32Matrix &mat);
   int32_t getDiagonalProduct(const Int32Matrix &A) const;
   void setIdentity();
   void triangularize(Int32Matrix &A, Int32Matrix &I, const uint32_t k, const uint32_t lower_i, const uint32_t upper_i) const;
   
   // Dimension of the square matrix.
   uint32_t dim;

   // modulo from Z_n.
   int32_t n = 1;

   Int32Matrix M;
};

#endif