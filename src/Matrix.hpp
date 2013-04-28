/*
 * Matrices carrées dans M(Z_n)
 */
#ifndef MATRIX_HPP
#define	MATRIX_HPP

#include <vector>

template <class T>
class Matrix
{
public:
   Matrix();
   virtual ~Matrix() {}
   
   T& operator()(const unsigned int r, const unsigned int c)
   {
      if (!p.empty() && r > 0 && r <= rows && c > 0 && c <= cols)
      {
         return p[r - 1][c - 1];
      }
      else
      {
         throw "Subscript out of range";
      }
   }
   
   // index operator. You can use this class like myMatrix.get(col, row)
   // the indexes are one-based, not zero based.
   // use this function get if you want to read from a const MatrixBase

   double get(const unsigned int r, const unsigned int c) const
   {
      if (!p.empty() && r > 0 && r <= rows && c > 0 && c <= cols)
      {
         return p[r - 1][c - 1];
      }
      else
      {
         throw "Subscript out of range";
      }
   }
   
   // addition of MatrixBase with MatrixBase

   friend Matrix& operator+(const Matrix& a, const Matrix& b)
   {
      // check if the dimensions match
      if (a.rows == b.rows && a.cols == b.cols)
      {
         Matrix res(a.rows, a.cols);

         for (unsigned int r = 0; r < a.rows; r++)
         {
            for (unsigned int c = 0; c < a.cols; c++)
            {
               res.p[r][c] = a.p[r][c] + b.p[r][c];
            }
         }

         return res;
      }
      else
      {
         throw "Dimensions does not match";
      }
   }
   
protected:
   unsigned int rows;
   unsigned int cols;
   std::vector<std::vector<T> > p;
};

#endif	/* MATRIX_HPP */

