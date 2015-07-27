/*
 * Square matrix in M_k(Z_n).
 */
#ifndef SQUAREMATRIX_HPP
#define SQUAREMATRIX_HPP

#include "Types.hpp"

namespace CryptoGL
{
    class SquareMatrix
    {
    public:
        SquareMatrix() { }
        
        SquareMatrix(const Int64Matrix &M, const int64_t modulo);
        
        SquareMatrix(const uint32_t dim, const int64_t modulo)
            : dim(dim), n(modulo), M(dim, Int64Vector(dim, 0)) { }

        void setMatrix(const Int64Matrix &);
        Int64Matrix getMatrix() const
        {
            return M;
        };

        /* Get a specific value of the current matrix. */
        int64_t get(const uint32_t row, const uint32_t col) const;
        
        Int64Vector getRow(const uint32_t row) const;
        
        void swap(const uint32_t row1, const uint32_t row2);

        /* Set value to a specific cell of the matrix.*/
        void set(const uint32_t row, const uint32_t col, const int64_t value);
        
        void setDimension(const uint32_t);
        uint32_t getDimension() const
        {
            return dim;
        }
        
        void setModulo(const int64_t);
        int64_t getModulo() const
        {
            return n;
        };

        /* Multiply the current square matrix with a column-vector of the same dimension. */
        Int64Vector multiplyByVector(const Int64Vector &V) const;
        
        /* Multiply the current square matrix with another one of the same dimension. */
        SquareMatrix multiplyBy(const SquareMatrix &A) const;

        /* Set Identity to the matrix (M_3 = [[1,0,0] ; [0,1,0] ; [0,0,1]]). */
        void setIdentity();
        
        /* Return the matrix Identity I_k, where k is the dimension of I.*/
        static Int64Matrix identity(const uint32_t dimension);

        /* Return the determinant of the current matrix.*/
        int64_t det() const;

        /* Inverse of the current matrix using the Gauss-Jordan method.*/
        void inverse();

    private:
        /* Find the first pivot A(n,n) != 0 if it exists. Otherwise, return dim + 1. */
        uint32_t findNonZero(const uint32_t from) const;

        /* Check if the matrix is square (N x N). */
        static bool isSquare(const Int64Matrix &mat);

        /* Calculate the trace of the matrix (vlaue of tr(M)). */
        int64_t trace() const;
        static int64_t trace(const Int64Matrix &A);

        void triangularize(SquareMatrix &I, const uint32_t k, const uint32_t lower_i, const uint32_t upper_i);

        // Dimension of the square matrix.
        uint32_t dim;

        // modulo from Z_n. n < 0 is possible because nZ = -nZ.
        int64_t n = 1;

        // The square matrix over Z_n.
        Int64Matrix M;
    };
}

#endif