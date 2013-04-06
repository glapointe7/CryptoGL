
#include "Matrix.hpp"

#include <iostream>

Matrix::Matrix()
{
   rows = 0;
   cols = 0;
}

Matrix::Matrix(const int rows, const int cols)
{
   if (rows > 0 && cols > 0)
   {
      this->rows = rows;
      this->cols = cols;

      for (int c = 0; c < cols; c++)
      {
         std::vector<double> row(rows, 0);
         p.push_back(row);
      }
   }
}

Matrix::Matrix(const Matrix& orig)
{
   rows = orig.rows;
   cols = orig.cols;
   p = orig.p;
}

double& Matrix::operator()(const int r, const int c)
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
// use this function get if you want to read from a const Matrix

double Matrix::get(const int r, const int c) const
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

Matrix& Matrix::operator=(const Matrix& a)
{
   rows = a.rows;
   cols = a.cols;
   p = a.p;
   
   return *this;
}

Matrix& Matrix::add(const double v)
{
   for (int r = 0; r < rows; r++)
   {
      for (int c = 0; c < cols; c++)
      {
         p[r][c] += v;
      }
   }
   
   return *this;
}

// subtract a double value (elements wise)

Matrix& Matrix::subtract(const double v)
{
   return add(-v);
}

// multiply a double value (elements wise)

Matrix& Matrix::multiply(const double v)
{
   for (int r = 0; r < rows; r++)
   {
      for (int c = 0; c < cols; c++)
      {
         p[r][c] *= v;
      }
   }
   
   return *this;
}

// divide a double value (elements wise)

Matrix& Matrix::divide(const double v)
{
   return multiply(1 / v);
}

// addition of Matrix with Matrix

Matrix operator+(const Matrix& a, const Matrix& b)
{
   // check if the dimensions match
   if (a.rows == b.rows && a.cols == b.cols)
   {
      Matrix res(a.rows, a.cols);

      for (int r = 0; r < a.rows; r++)
      {
         for (int c = 0; c < a.cols; c++)
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

// addition of Matrix with double

Matrix operator+(const Matrix& a, const double b)
{
   Matrix res = a;
   res.add(b);
   
   return res;
}
// addition of double with Matrix

 Matrix operator+(const double b, const Matrix& a)
{
   Matrix res = a;
   res.add(b);
   
   return res;
}

// subtraction of Matrix with Matrix

 Matrix operator-(const Matrix& a, const Matrix& b)
{
   // check if the dimensions match
   if (a.rows == b.rows && a.cols == b.cols)
   {
      Matrix res(a.rows, a.cols);

      for (int r = 0; r < a.rows; r++)
      {
         for (int c = 0; c < a.cols; c++)
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

// subtraction of Matrix with double

 Matrix operator-(const Matrix& a, const double b)
{
   Matrix res = a;
   res.subtract(b);
   
   return res;
}
// subtraction of double with Matrix

 Matrix operator-(const double b, const Matrix& a)
{
   Matrix res = -a;
   res.add(b);
   
   return res;
}

// operator unary minus

 Matrix operator-(const Matrix& a)
{
   Matrix res(a.rows, a.cols);

   for (int r = 0; r < a.rows; r++)
   {
      for (int c = 0; c < a.cols; c++)
      {
         res.p[r][c] = -a.p[r][c];
      }
   }

   return res;
}

// operator multiplication

 Matrix operator*(const Matrix& a, const Matrix& b)
{
   // check if the dimensions match
   if (a.cols == b.rows)
   {
      Matrix res(a.rows, b.cols);

      for (int r = 0; r < a.rows; r++)
      {
         for (int c_res = 0; c_res < b.cols; c_res++)
         {
            for (int c = 0; c < a.cols; c++)
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

// multiplication of Matrix with double

 Matrix operator*(const Matrix& a, const double b)
{
   Matrix res = a;
   res.multiply(b);
   
   return res;
}
// multiplication of double with Matrix

 Matrix operator*(const double b, const Matrix& a)
{
   Matrix res = a;
   res.multiply(b);
   
   return res;
}

// division of Matrix with Matrix

 Matrix operator/(const Matrix& a, const Matrix& b)
{
   // check if the dimensions match: must be square and equal sizes
   if (a.rows == a.cols && a.rows == a.cols && b.rows == b.cols)
   {
      Matrix res(a.rows, a.cols);
      res = a * Matrix::inverse(b);

      return res;
   }
   else
   {
      throw "Dimensions does not match";
   }
}

// division of Matrix with double

 Matrix operator/(const Matrix& a, const double b)
{
   Matrix res = a;
   res.divide(b);
   
   return res;
}

// division of double with Matrix

 Matrix operator/(const double b, const Matrix& a)
{
   Matrix b_matrix(1, 1);
   b_matrix(1, 1) = b;
   Matrix res = b_matrix / a;
   
   return res;
}

/**
 * returns the minor from the given matrix where
 * the selected row and column are removed
 */
Matrix Matrix::getMinor(const int row, const int col) const
{
   Matrix res;
   if (row > 0 && row <= rows && col > 0 && col <= cols)
   {
      res = Matrix(rows - 1, cols - 1);

      // copy the content of the matrix to the minor, except the selected
      for (int r = 1; r <= (rows - (row >= rows)); r++)
      {
         for (int c = 1; c <= (cols - (col >= cols)); c++)
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
int Matrix::size(const int i) const
{
   switch (i)
   {
      case 1: return rows;
      case 2: return cols;
   }
   
   return 0;
}

// returns the number of rows

int Matrix::getRows() const
{
   return rows;
}

// returns the number of columns

int Matrix::getCols() const
{
   return cols;
}

// print the contents of the matrix

void Matrix::print() const
{
   if (!p.empty())
   {
      std::cout << "[";
      for (int r = 0; r < rows; r++)
      {
         if (r > 0)
         {
            std::cout << " ";
         }
         for (int c = 0; c < cols - 1; c++)
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
int Matrix::size(const Matrix& a, const int i)
{
   return a.size(i);
}

/**
 * returns a matrix with size cols x rows with ones as values
 */
Matrix Matrix::ones(const int rows, const int cols)
{
   Matrix res = Matrix(rows, cols);

   for (int r = 1; r <= rows; r++)
   {
      for (int c = 1; c <= cols; c++)
      {
         res(r, c) = 1;
      }
   }
   return res;
}

/**
 * returns a matrix with size cols x rows with zeros as values
 */
Matrix Matrix::zeros(const int rows, const int cols)
{
   return Matrix(rows, cols);
}

/**
 * returns a diagonal matrix with size n x n with ones at the diagonal
 * @param  v a vector
 * @return a diagonal matrix with ones on the diagonal
 */
Matrix Matrix::diag(const int n)
{
   Matrix res = Matrix(n, n);
   for (int i = 1; i <= n; i++)
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
Matrix Matrix::diag(const Matrix& v)
{
   Matrix res;
   if (v.getCols() == 1)
   {
      // the given matrix is a vector n x 1
      int rows = v.getRows();
      res = Matrix(rows, rows);

      // copy the values of the vector to the matrix
      for (int r = 1; r <= rows; r++)
      {
         res(r, r) = v.get(r, 1);
      }
   }
   else if (v.getRows() == 1)
   {
      // the given matrix is a vector 1 x n
      int cols = v.getCols();
      res = Matrix(cols, cols);

      // copy the values of the vector to the matrix
      for (int c = 1; c <= cols; c++)
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

/*
 * returns the determinant of Matrix a
 */
double Matrix::det(const Matrix& a)
{
   double d = 0; // value of the determinant
   int rows = a.getRows();
   int cols = a.getRows();

   if (rows == cols)
   {
      // this is a square matrix
      if (rows == 1)
      {
         // this is a 1 x 1 matrix
         d = a.get(1, 1);
      }
      else if (rows == 2)
      {
         // this is a 2 x 2 matrix
         // the determinant of [a11,a12;a21,a22] is det = a11*a22-a21*a12
         d = a.get(1, 1) * a.get(2, 2) - a.get(2, 1) * a.get(1, 2);
      }
      else
      {
         // this is a matrix of 3 x 3 or larger
         for (int c = 1; c <= cols; c++)
         {
            Matrix M = a.getMinor(1, c);
            //d += pow(-1, 1+c) * a(1, c) * Det(M);
            d += (c % 2 + c % 2 - 1) * a.get(1, c) * det(M); // faster than with pow()
         }
      }
   }
   else
   {
      throw "Matrix must be square";
   }
   return d;
}

// swap two values

void Matrix::swap(double& a, double& b)
{
   double temp = a;
   a = b;
   b = temp;
}

/*
 * returns the inverse of Matrix a
 */
Matrix Matrix::inverse(const Matrix& a)
{
   Matrix res;
   double d = 0; // value of the determinant
   int rows = a.getRows();
   int cols = a.getRows();

   d = det(a);
   if (rows == cols && d != 0)
   {
      // this is a square matrix
      if (rows == 1)
      {
         // this is a 1 x 1 matrix
         res = Matrix(rows, cols);
         res(1, 1) = 1 / a.get(1, 1);
      }
      else if (rows == 2)
      {
         // this is a 2 x 2 matrix
         res = Matrix(rows, cols);
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
         Matrix ai = a; // make a copy of Matrix a

         for (int c = 1; c <= cols; c++)
         {
            // element (c, c) should be non zero. if not, swap content
            // of lower rows
            int r;
            for (r = c; r <= rows && ai(r, c) == 0; r++)
            {
            }
            if (r != c)
            {
               // swap rows
               for (int s = 1; s <= cols; s++)
               {
                  swap(ai(c, s), ai(r, s));
                  swap(res(c, s), res(r, s));
               }
            }

            // eliminate non-zero values on the other rows at column c
            for (int r = 1; r <= rows; r++)
            {
               if (r != c)
               {
                  // eleminate value at column c and row r
                  if (ai(r, c) != 0)
                  {
                     double f = -ai(r, c) / ai(c, c);

                     // add (f * row c) to row r to eleminate the value
                     // at column c
                     for (int s = 1; s <= cols; s++)
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
                  double f = ai(c, c);
                  for (int s = 1; s <= cols; s++)
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
         throw "Matrix must be square";
      }
      else
      {
         throw "Determinant of matrix is zero";
      }
   }
   return res;
}