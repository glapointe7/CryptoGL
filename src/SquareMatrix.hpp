/*
 * Square matrix in M_k(Z_n).
 */
#ifndef SQUAREMATRIX_HPP
#define SQUAREMATRIX_HPP

#include <vector>
#include <stdint.h>

class SquareMatrix
{
public:
   typedef std::vector<std::vector<int32_t> > Matrix;
   
   virtual ~SquareMatrix() {}

   void setMatrix(const Matrix &);
   const Matrix getMatrix() const;

   const int32_t get(const uint32_t row, const uint32_t col) const;
   void set(const uint32_t row, const uint32_t col, const int32_t value);

   const uint32_t getDimension() const { return dim; }
   void setDimension(const uint32_t);
   
   const int32_t getModulo() const;
   void setModulo(const int32_t);

   /* Multiply a square matrix with a vector of the same dimension. */
   friend const std::vector<uint32_t> operator *(const SquareMatrix *K, const std::vector<uint32_t> &V);

   const Matrix identity() const;
   
   int32_t det() const;
   const SquareMatrix* inverse() const;
   
private:
   uint32_t findNonZero(const Matrix &A, const uint32_t from) const;
   static bool isSquare(const Matrix &mat);
   int32_t getDiagonalProduct(const Matrix &A) const;
   void setIdentity();
   void triangularize(Matrix &A, Matrix &I, const uint32_t k, const uint32_t lower_i, const uint32_t upper_i) const;
   
   // Dimension of the square matrix.
   uint32_t dim;

   // modulo from Z_n.
   int32_t n = 1;

   Matrix M;
};

#endif