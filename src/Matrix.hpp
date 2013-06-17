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
   virtual ~Matrix() {}

   void setMatrix(const Matrice &);
   const Matrice getMatrix() const;

   const int32_t get(const uint32_t row, const uint32_t col) const;
   void set(const uint32_t row, const uint32_t col, const int32_t value);

   const uint32_t getDimension() const;
   void setDimension(const uint32_t);
   
   const int32_t getModulo() const;
   void setModulo(const int32_t);

   friend const std::vector<uint32_t> operator *(const Matrix *K, const std::vector<uint32_t> &V);

   const Matrice identity() const;
   
   int32_t det() const;
   const Matrix* inverse() const;
   
private:
   uint32_t findNonZero(const Matrice &A, const uint32_t from) const;
   static bool isSquare(const Matrice &mat);
   int32_t getDiagonalProduct(const Matrice &A) const;
   void setIdentity();
   void triangularize(Matrice &A, Matrice &I, const uint32_t k, const uint32_t lower_i, const uint32_t upper_i) const;
   
   // Dimension de la matrice carrée.
   uint32_t dim;

   // Matrice d'entiers modulo n (M est dans GL_dim(Z_n)).
   int32_t n = 1;

   // Matrice
   Matrice M;
};

#endif