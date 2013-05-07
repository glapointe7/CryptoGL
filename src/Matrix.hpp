/*
 * Square matrix in the group M(Z_n).
 */
#ifndef MATRIX_HPP
#define	MATRIX_HPP

#include <vector>
#include <stdint.h>

class Matrix
{
   typedef std::vector<std::vector<int32_t> > Matrice;

public:

   Matrix() {}
   Matrix(const Matrix &A);
   virtual ~Matrix() {}

   void setMatrix(const Matrice &);
   const Matrice getMatrix() const;

   const int32_t get(const uint32_t row, const uint32_t col) const;
   void set(const uint32_t row, const uint32_t col, const int32_t value);

   const uint32_t getDimension() const;
   void setDimension(const uint32_t);
   const int32_t getModulo() const;
   void setModulo(const int32_t);

   friend Matrix operator +(const Matrix& a, const Matrix& b);
   friend std::vector<uint32_t> operator *(const Matrix *K, const std::vector<uint32_t> &V);

   void swapRows(const uint32_t r1, const uint32_t r2);
   Matrix getMinor(const uint32_t row, const uint32_t col) const;
   void setIdentity();
   void zeros();
   
   int32_t getTrace() const;
   int32_t getDiagonalProduct() const;
   int32_t det() const;
   const Matrix* inverse() const;
   
private:
   uint32_t findNonZero(const uint32_t from) const;
   static bool isSquare(const Matrice &mat);
   
   // Dimension de la matrice carrée.
   uint32_t dim = 0;

   // Matrice d'entiers modulo n (M est dans GL_dim(Z_n)).
   int32_t n = 1;

   // Matrice
   Matrice M;
};

#endif