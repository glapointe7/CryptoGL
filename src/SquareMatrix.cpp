
#include "SquareMatrix.hpp"

// Exceptions
#include "exceptions/EmptyMatrix.hpp"
#include "exceptions/MatrixNotSquare.hpp"
#include "MathematicalTools.hpp"
#include "exceptions/MatrixOutOfRange.hpp"

#include <algorithm>  // swap

// Exceptions : Matrix have to be square and not empty.

void SquareMatrix::setMatrix(const Matrix &M)
{
   if (M.empty())
   {
      throw EmptyMatrix("The matrix is empty.");
   }
   
   if (!isSquare(M))
   {
      throw MatrixNotSquare("The matrix is not square.");
   }
   
   this->M = M;
   setDimension(M.size());
}

const SquareMatrix::Matrix SquareMatrix::getMatrix() const
{
   return M;
}

void SquareMatrix::setDimension(const uint32_t dim)
{
   this->dim = dim;
}

const int32_t SquareMatrix::getModulo() const
{
   return n;
}

void SquareMatrix::setModulo(const int32_t n)
{
   this->n = n;
}

// Get a specific value of the current matrix.

const int32_t SquareMatrix::get(const uint32_t row, const uint32_t col) const
{
   if (row < dim && col < dim)
   {
      return M[row][col];
   }

   throw MatrixOutOfRange("The row and/or column selected are out of range.");
}

// Set value to M(row,col).

void SquareMatrix::set(const uint32_t row, const uint32_t col, const int32_t value)
{
   if (row < dim && col < dim)
   {
      M[row][col] = value;
   }

   throw MatrixOutOfRange("The row and/or column selected are out of range.");
}

// Find the first pivot A(n,n) != 0 if it exists.
// Otherwise, return dim + 1.

uint32_t SquareMatrix::findNonZero(const Matrix &A, const uint32_t from) const
{
   uint32_t pos = from;
   while ((A[pos][from] == 0 || Maths::gcd(A[pos][from], n) != 1) && pos != dim)
   {
      pos++;
   }

   return pos;
}

// Multiply a matrix and a column-vector. Return the column-vector solution.

const std::vector<uint32_t> operator *(const SquareMatrix *K, const std::vector<uint32_t> &V)
{   
   std::vector<uint32_t> soln(K->getDimension(), 0);
   const int32_t mod = K->getModulo();
   const std::vector<std::vector<int32_t> > mat = K->getMatrix();
   
   uint32_t i = 0;
   for (const auto row : mat)
   {
      uint32_t j = 0;
      for (const auto number : row)
      {
         soln[i] += number * V[j++];
      }
      soln[i++] %= mod;
   }

   return soln;
}

// Initialize to the identity matrix.

void SquareMatrix::setIdentity()
{
   M = Matrix(dim, std::vector<int32_t>(dim, 0));
   for (uint32_t i = 0; i < dim; ++i)
   {
      M[i][i] = 1;
   }
}

// Return the matrix Identity.

const SquareMatrix::Matrix SquareMatrix::identity() const
{
   Matrix Mat = Matrix(dim, std::vector<int32_t>(dim, 0));
   for (uint32_t i = 0; i < dim; ++i)
   {
      Mat[i][i] = 1;
   }

   return Mat;
}

// Return diagonal product.

int32_t SquareMatrix::getDiagonalProduct(const Matrix &A) const
{
   int32_t prod = 1;
   for (uint32_t i = 0; i < dim; ++i)
   {
      prod *= A[i][i];
   }

   return prod;
}

// Check if the matrix mat is square.

bool SquareMatrix::isSquare(const Matrix &mat)
{
   const uint32_t A_size = mat.size();
   for (const auto V : mat)
   {
      if (V.size() != A_size)
      {
         return false;
      }
   }

   return true;
}

void SquareMatrix::triangularize(Matrix &A, Matrix &I, const uint32_t k, const uint32_t lower_i, const uint32_t upper_i) const
{
   // Swap null pivot with a non null one.
   if (A[k][k] == 0 || !Maths::areCoprimes(A[k][k], n))
   {
      const uint32_t pivot = findNonZero(A, k);
      std::swap(A[pivot], A[k]);
      std::swap(I[pivot], I[k]);
   }

   const int32_t inv = Maths::getModInverse(A[k][k], n);
   for (uint32_t i = lower_i; i < upper_i; ++i)
   {
      // For rows : Li = Li + lq*Ln.
      const int32_t lq = (inv * (n - A[i][k])) % n;
      for (uint32_t j = 0; j < dim; ++j)
      {
         A[i][j] = (A[i][j] + (lq * A[k][j])) % n;
         I[i][j] = (I[i][j] + (lq * I[k][j])) % n;
      }
   }
}

// Return the determinant of the current matrix.

int32_t SquareMatrix::det() const
{
   int32_t determinant = 0;

   switch (dim)
   {
      case 1:
      {
         determinant = M[0][0];
         break;
      }

      case 2:
      {
         determinant = ((M[0][0] * M[1][1]) - (M[0][1] * M[1][0]));
         break;
      }

      default:
      {
         Matrix A = M;
         int8_t swapping = 1;

         for (uint32_t k = 0; k < dim - 1; ++k)
         {
            // Swap the zero pivot with a non zero one.
            // If no one is found, then the column k below A(k,k) is zero.
            // Thus, det(M) = 0.
            if (A[k][k] == 0 || !Maths::areCoprimes(A[k][k], n))
            {
               const uint32_t pivot = findNonZero(A, k);
               if (pivot == dim)
               {
                  return 0;
               }
               std::swap(A[pivot], A[k]);
               swapping *= -1;
            }

            // Li = Li + q*Ln.
            const int32_t inv = Maths::getModInverse(A[k][k], n);
            for (uint32_t i = k + 1; i < dim; ++i)
            {
               const int32_t q = (inv * (n - A[i][k])) % n;
               for (uint32_t j = k + 1; j < dim; ++j)
               {

                  A[i][j] = (A[i][j] + (q * A[k][j])) % n;
               }
            }
         }
         determinant = getDiagonalProduct(A) * swapping;
      }
   }

   return determinant;
}

// Return A^-1 using the Gauss-Jordan method.

const SquareMatrix* SquareMatrix::inverse() const
{
   SquareMatrix *result = new SquareMatrix();
   result->setModulo(n);
   Matrix A;

   // Determinant of A is positive and is in the set {0,...,mod_A-1}
   const int32_t deter = det();

   // We make sure that GCD(det(result), mod) = 1 => result is reversible in the Z_mod group.
   if (Maths::areCoprimes(deter, n))
   {
      switch (dim)
      {
         case 1:
         {
            A = {
               {(Maths::getModInverse(deter, n) + n) % n}
            };
            result->setMatrix(A);
            break;
         }

         case 2:
         {
            const int32_t det_inv = Maths::getModInverse(deter, n);
            // Transform to a positive matrix.
            A = {
               {(((det_inv * M[1][1]) % n) + n) % n, (((det_inv * -M[0][1]) % n) + n) % n},
               {(((det_inv * -M[1][0]) % n) + n) % n, (((det_inv * M[0][0]) % n) + n) % n}
            };
            result->setMatrix(A);
            break;
         }

         default:
         {
            A = M;
            Matrix I = identity();

            // Triangular inferior.
            for (uint32_t k = 0; k < dim - 1; ++k)
            {
               triangularize(A, I, k, k+1, dim);
            }
            
            // Triangular superior.
            for (uint32_t k = dim - 1; k >= 1; --k)
            {
               triangularize(A, I, k, 0, k);
            }

            // Now we transform A to the identity matrix.
            for (uint32_t k = 0; k < dim; ++k)
            {
               const int32_t inv = Maths::getModInverse(A[k][k], n);
               for (uint32_t i = 0; i < dim; ++i)
               {
                  I[k][i] = (I[k][i] * inv) % n;
               }
            } // end FOR k
            result->setMatrix(I);
         } // end Default
      } // end Switch
   } // end IF

   return result;
}