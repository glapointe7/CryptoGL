/*
With this class you can:
  - create a 2D matrix with custom size
  - get/set the cell values
  - use operators +, -, *, /
  - use functions Ones(), Zeros(), Diag(), Det(), Inv(), Size()
  - print the content of the matrix

Usage:
  you can create a matrix by:
    MatrixBase A;
    MatrixBase A = MatrixBase(rows, cols);
    MatrixBase A = B;

  you can get and set matrix elements by:
    A(2,3) = 5.6;    // set an element of Matix A
    value = A(3,1);   // get an element of MatrixBase A
    value = A.get(3,1); // get an element of a constant MatrixBase A
    A = B;        // copy content of MatrixBase B to MatrixBase A

  you can apply operations with matrices and doubles:
    A = B + C;
    A = B - C;
    A = -B;
    A = B * C;
    A = B / C;

  the following functions are available:
    A = Ones(rows, cols);
    A = Zeros(rows, cols);
    A = Diag(n);
    A = Diag(B);
    d = Det(A);
    A = Inv(B);
    cols = A.GetCols();
    rows = A.GetRows();
    cols = Size(A, 1);
    rows = Size(A, 2);

  you can quick-print the content of a matrix in the console with:
    A.Print();
 */

#ifndef MATRIXBASE_HPP
#define	MATRIXBASE_HPP

#include "Matrix.hpp"

#include <vector>
#include <iostream>

template <class T>
class MatrixBase : Matrix<T>
{
public:

   MatrixBase()
   {
      rows = 0;
      cols = 0;
   }

   MatrixBase(const unsigned int rows, const unsigned int cols)
   {
      if (rows > 0 && cols > 0)
      {
         this->rows = rows;
         this->cols = cols;

         for (unsigned int c = 0; c < cols; c++)
         {
            std::vector<T> row(rows, 0);
            p.push_back(row);
         }
      }
   }

   MatrixBase(const MatrixBase& orig)
   {
      rows = orig.rows;
      cols = orig.cols;
      p = orig.p;
   }

   virtual ~MatrixBase()
   {
   }

   MatrixBase& operator=(const MatrixBase& a)
   {
      rows = a.rows;
      cols = a.cols;
      p = a.p;

      return *this;
   }

   MatrixBase& add(const T v)
   {
      for (unsigned int r = 0; r < rows; r++)
      {
         for (unsigned int c = 0; c < cols; c++)
         {
            p[r][c] += v;
         }
      }

      return *this;
   }

   // subtract a double value (elements wise)

   MatrixBase& subtract(const T v)
   {
      return add(-v);
   }

   // multiply a double value (elements wise)

   MatrixBase& multiply(const T v)
   {
      for (unsigned int r = 0; r < rows; r++)
      {
         for (unsigned int c = 0; c < cols; c++)
         {
            p[r][c] *= v;
         }
      }

      return *this;
   }

   // divide a double value (elements wise)

   MatrixBase& divide(const T v)
   {
      return multiply(1 / v);
   }

   // addition of MatrixBase with MatrixBase

   friend MatrixBase operator+(const MatrixBase& a, const MatrixBase& b)
   {
      // check if the dimensions match
      if (a.rows == b.rows && a.cols == b.cols)
      {
         MatrixBase res(a.rows, a.cols);

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

   // addition of MatrixBase with double

   friend MatrixBase operator+(const MatrixBase& a, const T b)
   {
      MatrixBase res = a;
      res.add(b);

      return res;
   }

   // addition of double with MatrixBase

   friend MatrixBase operator+(const T b, const MatrixBase& a)
   {
      MatrixBase res = a;
      res.add(b);

      return res;
   }

   // subtraction of MatrixBase with MatrixBase

   friend MatrixBase operator-(const MatrixBase& a, const MatrixBase& b)
   {
      // check if the dimensions match
      if (a.rows == b.rows && a.cols == b.cols)
      {
         MatrixBase res(a.rows, a.cols);

         for (unsigned int r = 0; r < a.rows; r++)
         {
            for (unsigned int c = 0; c < a.cols; c++)
            {
               res.p[r][c] = a.p[r][c] - b.p[r][c];
            }
         }

         return res;
      }
      else
      {
         // give an error
         throw "Dimensions does not match";
      }
   }

   // subtraction of MatrixBase with double

   friend MatrixBase operator-(const MatrixBase& a, const T b)
   {
      MatrixBase res = a;
      res.subtract(b);

      return res;
   }

   // subtraction of double with MatrixBase

   friend MatrixBase operator-(const T b, const MatrixBase& a)
   {
      MatrixBase res = -a;
      res.add(b);

      return res;
   }

   // operator unary minus

   friend MatrixBase operator-(const MatrixBase& a)
   {
      MatrixBase res(a.rows, a.cols);

      for (unsigned int r = 0; r < a.rows; r++)
      {
         for (unsigned int c = 0; c < a.cols; c++)
         {
            res.p[r][c] = -a.p[r][c];
         }
      }

      return res;
   }

   // operator multiplication

   friend MatrixBase operator*(const MatrixBase& a, const MatrixBase& b)
   {
      // check if the dimensions match
      if (a.cols == b.rows)
      {
         MatrixBase res(a.rows, b.cols);

         for (unsigned int r = 0; r < a.rows; r++)
         {
            for (unsigned int c_res = 0; c_res < b.cols; c_res++)
            {
               for (unsigned int c = 0; c < a.cols; c++)
               {
                  res.p[r][c_res] += a.p[r][c] * b.p[c][c_res];
               }
            }
         }

         return res;
      }
      else
      {
         throw "Dimensions does not match";
      }
   }

   // multiplication of MatrixBase with double

   friend MatrixBase operator*(const MatrixBase& a, const T b)
   {
      MatrixBase res = a;
      res.multiply(b);

      return res;
   }

   // multiplication of double with MatrixBase

   friend MatrixBase operator*(const T b, const MatrixBase& a)
   {
      MatrixBase res = a;
      res.multiply(b);

      return res;
   }

   // division of MatrixBase with MatrixBase

   friend MatrixBase operator/(const MatrixBase& a, const MatrixBase& b)
   {
      // check if the dimensions match: must be square and equal sizes
      if (a.rows == a.cols && a.rows == a.cols && b.rows == b.cols)
      {
         MatrixBase res(a.rows, a.cols);
         res = a * MatrixBase::inverse(b);

         return res;
      }
      else
      {
         throw "Dimensions does not match";
      }
   }

   // division of MatrixBase with double

   friend MatrixBase operator/(const MatrixBase& a, const T b)
   {
      MatrixBase res = a;
      res.divide(b);

      return res;
   }

   // division of double with MatrixBase

   friend MatrixBase operator/(const T b, const MatrixBase& a)
   {
      MatrixBase b_matrix(1, 1);
      b_matrix(1, 1) = b;
      MatrixBase res = b_matrix / a;

      return res;
   }

   /**
    * returns the minor from the given matrix where
    * the selected row and column are removed
    */
   MatrixBase getMinor(const unsigned int row, const unsigned int col) const
   {
      MatrixBase res;
      if (row > 0 && row <= rows && col > 0 && col <= cols)
      {
         res = MatrixBase(rows - 1, cols - 1);

         // copy the content of the matrix to the minor, except the selected
         for (unsigned int r = 1; r <= (rows - (row >= rows)); r++)
         {
            for (unsigned int c = 1; c <= (cols - (col >= cols)); c++)
            {
               res(r - (r > row), c - (c > col)) = p[r - 1][c - 1];
            }
         }
      }
      else
      {
         throw "Index for minor out of range";
      }

      return res;
   }

   /*
    * returns the size of the i-th dimension of the matrix.
    * i.e. for i=1 the function returns the number of rows,
    * and for i=2 the function returns the number of columns
    * else the function returns 0
    */
   unsigned int size(const unsigned int i) const
   {
      switch (i)
      {
         case 1: return rows;
         case 2: return cols;
      }

      return 0;
   }

   unsigned int getRows() const
   {
      return rows;
   }

   unsigned int getCols() const
   {
      return cols;
   }

   // print the contents of the matrix

   void print() const
   {
      if (!p.empty())
      {
         std::cout << "[";
         for (unsigned int r = 0; r < rows; r++)
         {
            if (r > 0)
            {
               std::cout << " ";
            }
            for (unsigned int c = 0; c < cols - 1; c++)
            {
               std::cout << p[r][c] << ", ";
               //printf("%.2f, ", p[r][c]);
            }
            if (r < rows - 1)
            {
               std::cout << p[r][cols - 1] << ";\n";
               //printf("%.2f;\n", p[r][cols - 1]);
            }
            else
            {
               std::cout << p[r][cols - 1] << "]\n";
               //printf("%.2f]\n", p[r][cols - 1]);
            }
         }
      }
      else
      {
         // matrix is empty
         std::cout << "[ ]\n";
      }
   }

   /*
    * i.e. for i=1 the function returns the number of rows,
    * and for i=2 the function returns the number of columns
    * else the function returns 0
    */
   static unsigned int size(const MatrixBase& a, const unsigned int i)
   {
      return a.size(i);
   }

   // returns a matrix with size cols x rows with ones as values.

   static MatrixBase ones(const unsigned int rows, const unsigned int cols)
   {
      MatrixBase res = MatrixBase(rows, cols);

      for (unsigned int r = 1; r <= rows; r++)
      {
         for (unsigned int c = 1; c <= cols; c++)
         {
            res(r, c) = 1;
         }
      }
      return res;
   }

   // returns a matrix with size cols x rows with zeros as values

   static MatrixBase zeros(const unsigned int rows, const unsigned int cols)
   {
      return MatrixBase(rows, cols);
   }

   /**
    * returns a diagonal matrix with size n x n with ones at the diagonal
    * @param  v a vector
    * @return a diagonal matrix with ones on the diagonal
    */
   static MatrixBase diag(const unsigned int n)
   {
      MatrixBase res = MatrixBase(n, n);
      for (unsigned int i = 1; i <= n; i++)
      {
         res(i, i) = 1;
      }
      return res;
   }

   /**
    * returns a diagonal matrix
    * @param  v a vector
    * @return a diagonal matrix with the given vector v on the diagonal
    */
   static MatrixBase diag(const MatrixBase& v)
   {
      MatrixBase res;
      if (v.getCols() == 1)
      {
         // the given matrix is a vector n x 1
         unsigned int rows = v.getRows();
         res = MatrixBase(rows, rows);

         // copy the values of the vector to the matrix
         for (unsigned int r = 1; r <= rows; r++)
         {
            res(r, r) = v.get(r, 1);
         }
      }
      else if (v.getRows() == 1)
      {
         // the given matrix is a vector 1 x n
         unsigned int cols = v.getCols();
         res = MatrixBase(cols, cols);

         // copy the values of the vector to the matrix
         for (unsigned int c = 1; c <= cols; c++)
         {
            res(c, c) = v.get(1, c);
         }
      }
      else
      {
         throw "Parameter for diag must be a vector";
      }
      return res;
   }

   // returns the determinant of MatrixBase a

   static T det(const MatrixBase<T>& a)
   {
      T d = 0; // value of the determinant
      unsigned int rows = a.getRows();
      unsigned int cols = a.getRows();

      if (rows == cols)
      {
         // this is a square matrix
         switch (rows)
         {
            case 1:
               d = a.get(1, 1);
               break;

            case 2:
               d = a.get(1, 1) * a.get(2, 2) - a.get(2, 1) * a.get(1, 2);
               break;

            default:
               for (unsigned int c = 1; c <= cols; c++)
               {
                  MatrixBase M = a.getMinor(1, c);
                  //d += pow(-1, 1+c) * a(1, c) * Det(M);
                  d += (c % 2 + c % 2 - 1) * a.get(1, c) * det(M); // faster than with pow()
               }
         }
      }
      else
      {
         throw "MatrixBase must be square";
      }

      return d;
   }

   static void swap(T& a, T& b)
   {
      T temp = a;
      a = b;
      b = temp;
   }

   static MatrixBase inverse(const MatrixBase& a)
   {
      MatrixBase res;

      // value of the determinant
      T d = 0;
      unsigned int rows = a.getRows();
      unsigned int cols = a.getRows();

      d = det(a);

      if (typeid (d).name() == "float" || typeid (d).name() == "double")
      {
         if (rows == cols && d != 0)
         {
            // this is a square matrix
            if (rows == 1)
            {
               // this is a 1 x 1 matrix
               res = MatrixBase(rows, cols);
               res(1, 1) = 1 / a.get(1, 1);
            }
            else if (rows == 2)
            {
               // this is a 2 x 2 matrix
               res = MatrixBase(rows, cols);
               res(1, 1) = a.get(2, 2);
               res(1, 2) = -a.get(1, 2);
               res(2, 1) = -a.get(2, 1);
               res(2, 2) = a.get(1, 1);
               res = (1 / d) * res;
            }
            else
            {
               // this is a matrix of 3 x 3 or larger
               // calculate inverse using gauss-jordan elimination
               //   http://mathworld.wolfram.com/MatrixInverse.html
               //   http://math.uww.edu/~mcfarlat/inverse.htm
               res = diag(rows); // a diagonal matrix with ones at the diagonal
               MatrixBase ai = a; // make a copy of MatrixBase a

               for (unsigned int c = 1; c <= cols; c++)
               {
                  // element (c, c) should be non zero. if not, swap content
                  // of lower rows
                  unsigned int r;
                  for (r = c; r <= rows && ai(r, c) == 0; r++);
                  
                  if (r != c)
                  {
                     // swap rows
                     for (unsigned int s = 1; s <= cols; s++)
                     {
                        swap(ai(c, s), ai(r, s));
                        swap(res(c, s), res(r, s));
                     }
                  }

                  // eliminate non-zero values on the other rows at column c
                  for (unsigned int r = 1; r <= rows; r++)
                  {
                     if (r != c)
                     {
                        // eleminate value at column c and row r
                        if (ai(r, c) != 0)
                        {
                           T f = -ai(r, c) / ai(c, c);

                           // add (f * row c) to row r to eleminate the value
                           // at column c
                           for (unsigned int s = 1; s <= cols; s++)
                           {
                              ai(r, s) += f * ai(c, s);
                              res(r, s) += f * res(c, s);
                           }
                        }
                     }
                     else
                     {
                        // make value at (c, c) one,
                        // divide each value on row r with the value at ai(c,c)
                        T f = ai(c, c);
                        for (unsigned int s = 1; s <= cols; s++)
                        {
                           ai(r, s) /= f;
                           res(r, s) /= f;
                        }
                     }
                  }
               }
            }
         }
         else
         {
            if (rows == cols)
            {
               throw "MatrixBase must be square";
            }
            else
            {
               throw "Determinant of matrix is zero";
            }
         }
      }
      else
      {
         if (rows == cols && (d == 1 || d == -1))
         {

         }
      }
      return res;
   }

private:
   
};

#endif	/* MATRIX_HPP */