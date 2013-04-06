/*
With this class you can:
  - create a 2D matrix with custom size
  - get/set the cell values
  - use operators +, -, *, /
  - use functions Ones(), Zeros(), Diag(), Det(), Inv(), Size()
  - print the content of the matrix

Usage:
  you can create a matrix by:
    Matrix A;
    Matrix A = Matrix(rows, cols);
    Matrix A = B;

  you can get and set matrix elements by:
    A(2,3) = 5.6;    // set an element of Matix A
    value = A(3,1);   // get an element of Matrix A
    value = A.get(3,1); // get an element of a constant Matrix A
    A = B;        // copy content of Matrix B to Matrix A

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

#ifndef MATRIX_HPP
#define	MATRIX_HPP

#include <vector>

class Matrix
{
public:
   Matrix();
   Matrix(const int row_count, const int column_count);
   Matrix(const Matrix& orig);
   virtual ~Matrix() {}

   double& operator()(const int r, const int c);
   double get(const int r, const int c) const;
   Matrix& operator=(const Matrix& a);

   Matrix& add(const double v);
   Matrix& subtract(const double v);
   Matrix& multiply(const double v);
   Matrix& divide(const double v);

   friend Matrix operator+(const Matrix& a, const Matrix& b);
   friend Matrix operator+(const Matrix& a, const double b);
   friend Matrix operator+(const double b, const Matrix& a);
   friend Matrix operator-(const Matrix& a, const Matrix& b);
   friend Matrix operator-(const Matrix& a, const double b);
   friend Matrix operator-(const double b, const Matrix& a);
   friend Matrix operator-(const Matrix& a);
   friend Matrix operator*(const Matrix& a, const Matrix& b);
   friend Matrix operator*(const Matrix& a, const double b);
   friend Matrix operator*(const double b, const Matrix& a);
   friend Matrix operator/(const Matrix& a, const Matrix& b);
   friend Matrix operator/(const Matrix& a, const double b);
   friend Matrix operator/(const double b, const Matrix& a);

   Matrix getMinor(const int row, const int col) const;
   int size(const int i) const;
   int getRows() const;
   int getCols() const;
   void print() const;
   
   static int size(const Matrix& a, const int i);
   static Matrix ones(const int rows, const int cols);
   static Matrix zeros(const int rows, const int cols);
   static Matrix diag(const int n);
   static Matrix diag(const Matrix& v);
   static double det(const Matrix& a);
   static void swap(double& a, double& b);
   static Matrix inverse(const Matrix& a);

private:
   int rows;
   int cols;
   std::vector<std::vector<double> > p;
};

#endif	/* MATRIX_HPP */