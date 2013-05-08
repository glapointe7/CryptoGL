/*
 * TODO : Code redondant dans inverse et det.
 */
#include "Matrix.hpp"

// Exceptions
#include "exceptions/EmptyMatrix.hpp"
#include "exceptions/MatrixNotSquare.hpp"
#include "MathematicalTools.hpp"
#include "exceptions/MatrixOutOfRange.hpp"

#include <algorithm>  // swap

Matrix::Matrix(const Matrix &A)
{
   n = A.n;
   dim = A.dim;
   M = A.M;
}

// Exceptions : Matrix have to be square and not empty.

void Matrix::setMatrix(const Matrice &M)
{
   if (M.empty())
   {
      throw EmptyMatrix("The matrix is empty.");
   }
   else if (!isSquare(M))
   {
      throw MatrixNotSquare("The matrix is not square.");
   }
   else
   {
      this->M = M;
      setDimension(M.size());
   }
}

const Matrix::Matrice Matrix::getMatrix() const
{
   return M;
}

const uint32_t Matrix::getDimension() const
{
   return dim;
}

void Matrix::setDimension(const uint32_t dim)
{
   this->dim = dim;
}

const int32_t Matrix::getModulo() const
{
   return n;
}

void Matrix::setModulo(const int32_t n)
{
   this->n = n;
}

// Get a specific value of the current matrix.

const int32_t Matrix::get(const uint32_t row, const uint32_t col) const
{
   if (row < dim && col < dim)
   {
      return M[row][col];
   }
   
   throw MatrixOutOfRange("The row and/or column selected are out of range.");
}

// Set value to M(row,col).

void Matrix::set(const uint32_t row, const uint32_t col, const int32_t value)
{
   if (row < dim && col < dim)
   {
      M[row][col] = value;
   }

   throw MatrixOutOfRange("The row and/or column selected are out of range.");
}

// Swap 2 rows of the current matrix.

void Matrix::swapRows(const uint32_t r1, const uint32_t r2)
{
   std::swap(M[r1], M[r2]);
}

// Trouve le premier pivot A(n,n) != 0 s'il existe.
// Si le pivot n'existe pas, alors ça retourne dim + 1.

uint32_t Matrix::findNonZero(const uint32_t from) const
{
   uint32_t pos = from;
   while (M[pos][from] == 0 && pos != dim)
   {
      pos++;
   }

   return pos;
}

Matrix operator+(const Matrix &A, const Matrix &B)
{
   if (A.getDimension() == 0 || B.getDimension() == 0)
   {
      throw EmptyMatrix("Dimension of the matrices should not be zero.");
   }
   else if (A.getDimension() != B.getDimension())
   {
      throw MatrixNotSquare("Both matrices should be the same dimension.");
   }
   else
   {
      Matrix result;
      const uint32_t dim_A = A.getDimension();
      result.setDimension(dim_A);
      for (uint32_t row = 0; row < dim_A; row++)
      {
         for (uint32_t col = 0; col < dim_A; col++)
         {
            result.set(row, col, A.get(row, col) + B.get(row, col));
         }
      }

      return result;
   }
}

// Multiply a matrix and a column-vector. Return the column-vector solution.

std::vector<uint32_t> operator *(const Matrix *K, const std::vector<uint32_t> &V)
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
         soln[i] += number * V[j];
         j++;
      }
      soln[i] %= mod;
      i++;
   }

   return soln;
}

// Retourne le mineur d'ordre dim -1 de M.
// row et col sont la ligne et la colonne enlevées pour calculer le mineur.

Matrix Matrix::getMinor(const uint32_t row, const uint32_t col) const
{
   Matrix result;
   if (row > 0 && row <= dim && col > 0 && col <= dim)
   {
      result.setDimension(dim - 1);

      // Enlève la ligne / colonne sélectionnée et copie le reste de la matrice dans le mineur.
      for (uint32_t r = 1; r <= (dim - (row >= dim)); r++)
      {
         for (uint32_t c = 1; c <= (dim - (col >= dim)); c++)
         {
            result.set(r - (r > row), c - (c > col), M[r - 1][c - 1]);
         }
      }
   }
   else
   {
      throw MatrixOutOfRange("Index for minor is out of range");
   }

   return result;
}

// Initialize to the identity matrix.

void Matrix::setIdentity()
{
   M = Matrice(dim, std::vector<int32_t>(dim, 0));
   for (uint32_t i = 0; i < dim; ++i)
   {
      M[i][i] = 1;
   }
}

// Initialize to the zero matrix.

void Matrix::zeros()
{
   M = Matrice(dim, std::vector<int32_t>(dim, 0));
}

// Get the trace of the current matrix : tr(A) = somme(a_{i,i}).

int32_t Matrix::getTrace() const
{
   int32_t tr = 0;
   for (uint32_t i = 0; i < dim; ++i)
   {
      tr += M[i][i];
   }

   return tr;
}

// Return diagonal product.

int32_t Matrix::getDiagonalProduct() const
{
   int32_t prod = 1;
   for (uint32_t i = 0; i < dim; ++i)
   {
      prod *= M[i][i];
   }

   return prod;
}

// Check if the matrix mat is square.

bool Matrix::isSquare(const Matrice &mat)
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

// Return the determinant of the current matrix.

int32_t Matrix::det() const
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
         determinant = ((M[0][0] * M[1][1]) - (M[0][1] * M[1][0])) % n;
         break;
      }

      default:
      {
         Matrix *A = new Matrix(*this);
         const int32_t mod = A->getModulo();
         const uint32_t dimension = A->getDimension();
         
         for (uint32_t k = 0; k < dimension-1; ++k)
         {
            // Swap the zero pivot with a non zero one.
            // If no one is found, then the column k below A(k,k) is zero.
            // Thus, det(M) = 0.
            if (A->get(k, k) == 0)
            {
               const uint32_t pivot = A->findNonZero(k);
               if (pivot == dimension)
               {
                  return 0;
               }
               A->swapRows(pivot, k);
            }

            // Li = Li + q*Ln.
            const uint32_t inv = getModInverse(A->get(k, k), mod);
            for (uint32_t i = k + 1; i < dimension; ++i)
            {
               const int32_t q = (inv * (mod - A->get(i, k))) % mod;
               for (uint32_t j = k + 1; j < dimension; ++j)
               {
                  
                  A->set(i, j, (A->get(i, j) + (q * A->get(k, j))) % mod);
               }
            }
         }
         determinant = A->getDiagonalProduct() % mod;
         delete A;
      }
   }

   return determinant;
}

// Return A^-1 using the Gauss-Jordan method.

const Matrix* Matrix::inverse() const
{
   Matrix *result = new Matrix(*this);
   const int32_t mod = result->getModulo();
   const uint32_t dimension = result->getDimension();

   // Determinant of A is positive and is in the set {0,...,mod_A-1}
   const int32_t deter = (result->det() + mod) % mod;

   // We make sure that GCD(det(result), mod) = 1 => result is reversible in the Z_mod group.
   if (GCD(deter, mod) == 1)
   {
      switch (dimension)
      {
         case 1:
         {
            result->set(0, 0, getModInverse(deter, mod));
            break;
         }

         case 2:
         {
            int32_t det_inv = getModInverse(deter, mod);
            result->set(0, 0, (det_inv * result->get(1, 1)) % mod);
            result->set(0, 1, (det_inv * result->get(0, 1)) % mod);
            result->set(1, 0, (det_inv * result->get(1, 0)) % mod);
            result->set(1, 1, (det_inv * result->get(0, 0)) % mod);
            break;
         }

         default:
         {
            Matrix *A = new Matrix(*result);
            result->setIdentity();

            for (uint32_t k = 0; k < dimension-1; ++k)
            {
               // Swap null pivot with a non null one.
               if (A->get(k, k) == 0)
               {
                  A->swapRows(A->findNonZero(k), k);
                  result->swapRows(result->findNonZero(k), k);
               }

               const uint32_t inv = getModInverse(A->get(k, k), mod);
               for (uint32_t i = k + 1; i < dimension; ++i)
               {
                  // For rows : Li = Li + lq*Ln.
                  const int32_t lq = (inv * (mod - A->get(i, k))) % mod;
                  //const int32_t ilq = (inv * (mod_A - result(i, n))) % mod_A;

                  //const int32_t icq = (inv * (mod_A - result(n, i))) % mod_A;
                  for (uint32_t j = k; j < dimension; ++j)
                  {
                     A->set(i, j, (A->get(i, j) + (lq * A->get(k, j))) % mod);
                     result->set(i, j, result->get(i, j) + (lq * result->get(k, j)) % mod);
                  }
               }

               for (uint32_t i = k + 1; i < dimension; ++i)
               {
                  // For columns : Ci = Ci + cq*Ln.
                  const int32_t cq = (inv * (mod - A->get(k, i))) % mod;

                  for (uint32_t j = k; j < dimension; ++j)
                  {
                     A->set(j, i, (A->get(j, i) + (cq * A->get(j, k))) % mod);
                     result->set(j, i, result->get(j, i) + (cq * result->get(j, k)) % mod);
                  }
               }
            }

            // Now we transform A to the identity matrix.
            for (uint32_t k = 0; k < dimension; ++k)
            {
               const int32_t inv = getModInverse(A->get(k, k), mod);
               for (uint32_t i = 0; i < dimension; ++i)
               {
                  result->set(k, i, (result->get(k, i) * inv) % mod);
               }
            } // end FOR k
            delete A;
         } // end Default
      } // end Switch
   } // end IF

   return result;
}