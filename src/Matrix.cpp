#include "Matrix.hpp"

#include "EmptyMatrix.hpp"
#include "BadMatrix.hpp"
#include "MathematicalTools.hpp"

#include <algorithm>
#include <iostream>

// Exceptions : Matrice carrée, Matrice non vide
void Matrix::setMatrix(const Matrice &M)
{
   if(M.empty())
   {
      throw EmptyMatrix("The matrix is empty.");
   }
   else if(!isSquare(M))
   {
      throw BadMatrix("The matrix should be square.");
   }
   else
   {
      this->M = M;
      setDimension(M.size());
   }
}

const uint32_t Matrix::getDimension() const
{
   return dim;
}

// Initialisation à la matrice nulle selon la dimension choisie.

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

const int32_t Matrix::get(const uint32_t row, const uint32_t col) const
{
   if (row > 0 && row <= dim && col > 0 && col <= dim)
   {
      return M[row - 1][col - 1];
   }
   throw BadMatrix("Out of range.");
}

// Affecte une valeur en M(row,col).

void Matrix::set(const uint32_t row, const uint32_t col, const int32_t value)
{
   if (row > 0 && row <= dim && col > 0 && col <= dim)
   {
      M[row - 1][col - 1] = value;
   }
   else
   {
      throw BadMatrix("Out of range.");
   }
}

void Matrix::swapRows(const uint32_t r1, const uint32_t r2)
{
   std::swap(M[r1], M[r2]);
}

// Trouve le premier pivot A(n,n) != 0 s'il existe.
// Si le pivot n'existe pas, alors ça retourne dim + 1.

const uint32_t Matrix::findNonZero(const uint32_t from) const
{
   uint32_t pos = from - 1;
   while (M[pos][from - 1] == 0 && pos != dim)
   {
      pos++;
   }

   return pos;
}

// Obtention d'une valeur de la matrice.

const int32_t Matrix::operator()(const uint32_t row, const uint32_t col) const
{
   return get(row, col);
}

Matrix operator+(const Matrix &A, const Matrix &B)
{
   if (A.getDimension() == 0 || B.getDimension() == 0)
   {
      throw EmptyMatrix("Dimension of the matrix should not be zero.");
   }
   else if (A.getDimension() != B.getDimension())
   {
      throw BadMatrix("Both matrices should be the same dimension.");
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
            result.set(row, col, A(row, col) + B(row, col));
         }
      }

      return result;
   }
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
      throw BadMatrix("Index for minor out of range");
   }

   return result;
}

// Initialise à la matrice Identité.

void Matrix::setIdentity()
{
   M = Matrice(dim, std::vector<int32_t>(dim, 0));
   for (uint32_t i = 0; i < dim; ++i)
   {
      M[i][i] = 1;
   }
}

// Initialise à la matrice nulle.

void Matrix::zeros()
{
   M = Matrice(dim, std::vector<int32_t>(dim, 0));
}

// Calcule la trace de la matrice A : tr(A) = somme(a_{i,i}).

int32_t Matrix::trace(const Matrix &A)
{
   int32_t tr = 0;
   for (uint32_t i = 1; i <= A.getDimension(); ++i)
   {
      tr += A(i, i);
   }

   return tr;
}

// Calcule le produit des nombres sur la diagonale de A.

int32_t Matrix::diagonalProduct(const Matrix &A)
{
   int32_t prod = 1;
   for (uint32_t i = 1; i <= A.getDimension(); ++i)
   {
      prod *= A(i, i);
   }

   return prod;
}

// Vérifie si la matrice A est carrée.
bool Matrix::isSquare(const Matrice &A)
{
   const uint32_t A_size = A.size();
   for(auto V : A)
   {
      if(V.size() != A_size)
      {
         return false;
      }
   }
   
   return true;
}

// Retourne le déterminant de la matrice A donnée.

int32_t Matrix::det(Matrix &A)
{
   int32_t determinant = 0;
   const int32_t mod = A.getModulo();
   const uint32_t dim = A.getDimension();

   switch (dim)
   {
      case 1:
      {
         determinant = A(1u, 1u);
         break;
      }

      case 2:
      {
         determinant = ((A(1u, 1u) * A(2u, 2u)) - (A(1u, 2u) * A(2u, 1u))) % mod;
         break;
      }

      default:
      {
         for (uint32_t n = 1; n < dim; ++n)
         {
            // Si le pivot est nul, on doit l'échanger avec un autre non nul.
            // Si aucun pivot n'est trouvé, la colonne n ne contient que des 0 sous A(n,n).
            // Donc, det(A) = 0.
            if (A(n, n) == 0)
            {
               const uint32_t pivot = A.findNonZero(n) + 1;
               if (pivot == dim + 1)
               {
                  return 0;
               }
               A.swapRows(pivot - 1, n - 1);
            }

            // Li = Li + q*Ln.
            const uint32_t inv = getModInverse(A(n, n), mod);
            for (uint32_t i = n + 1; i <= dim; ++i)
            {
               const int32_t q = (inv * (mod - A(i, n))) % mod;
               for (uint32_t j = n + 1; j <= dim; ++j)
               {
                  A.set(i, j, (A(i, j) + (q * A(n, j))) % mod);
               }
            }
         }
         determinant = diagonalProduct(A) % mod;
      }
   }

   return determinant;
}

// Retourne l'inverse de la matrice A selon la méthode de Gauss-Jordan.

Matrix Matrix::inverse(Matrix &A)
{
   Matrix result;
   const int32_t mod_A = A.getModulo();
   const uint32_t dim_A = A.getDimension();

   // Le déterminant de A est positif et dans {0,...,mod_A-1}
   const int32_t det_A = (det(A) + mod_A) % mod_A;

   // On s'assure que PGCD(det(A), n) = 1 => A est inversible dans Z_n et det(A)^-1 existe.
   if (GCD(det_A, mod_A) == 1)
   {
      result.setDimension(dim_A);

      switch (dim_A)
      {
         case 1:
         {
            result.set(1u, 1u, getModInverse(det_A, mod_A));
            break;
         }

         case 2:
         {
            int32_t det_inv = getModInverse(det_A, mod_A);
            result.set(1u, 1u, (det_inv * A(2u, 2u)) % mod_A);
            result.set(1u, 2u, (det_inv * A(1u, 2u)) % mod_A);
            result.set(2u, 1u, (det_inv * A(2u, 1u)) % mod_A);
            result.set(2u, 2u, (det_inv * A(1u, 1u)) % mod_A);
            break;
         }

         default:
         {
            result.setIdentity();
            
            for (uint32_t n = 1; n < dim_A; ++n)
            {
               // Si le pivot est nul, on doit l'échanger avec un autre non nul.
               // Si aucun pivot n'est trouvé, la colonne n ne contient que des 0 sous A(n,n).
               // Donc, det(A) = 0.
               if (A(n, n) == 0)
               {
                  A.swapRows(A.findNonZero(n), n - 1);
                  result.swapRows(result.findNonZero(n), n-1);
               }

               // Li = Li + q*Ln.
               const uint32_t inv = getModInverse(A(n, n), mod_A);
               for (uint32_t i = n + 1; i <= dim_A; ++i)
               {
                  // Pour les lignes.
                  const int32_t lq = (inv * (mod_A - A(i, n))) % mod_A;
                  const int32_t ilq = (inv * (mod_A - result(i, n))) % mod_A;
                  
                  // Pour les colonnes.
                  const int32_t cq = (inv * (mod_A - A(n, i))) % mod_A;
                  const int32_t icq = (inv * (mod_A - result(n, i))) % mod_A;
                  for (uint32_t j = n; j <= dim_A; ++j)
                  {
                     A.set(i, j, (A(i, j) + (lq * A(n, j))) % mod_A);
                     result.set(i, j, result(i, j) + (ilq * result(n, j)) % mod_A);
                     
                     A.set(j, i, (A(j, i) + (cq * A(j, n))) % mod_A);
                     result.set(j, i, result(j, i) + (icq * result(j, n)) % mod_A);
                  }
               }
            }
            
            // On rend la matrice A = identité => des 1 dans la diagonale.
            for(uint32_t n = 1; n <= dim_A; ++n)
            {
               const int32_t inv = getModInverse(A(n, n), mod_A);
               for(uint32_t i = 1; i <= dim_A; ++i)
               {
                  result.set(n, i, (A(n, i) * inv) % mod_A);
               }
            } // fin FOR n
         } // fin Default
      } // fin Switch
   } // fin IF

   return result;
}

void Matrix::display(const Matrix &A)
{
   const uint32_t dim = A.getDimension();
   for(uint32_t i = 1; i <= dim; ++i)
   {
      for(uint32_t j = 1; j <= dim; ++j)
      {
         std::cout << A(i, j) << " ";
      }
      std::cout << "\n";
   }
}