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
        SquareMatrix(const uint32_t dim, const int32_t modulo)
            : dim(dim), n(modulo), M(dim, Int32Vector(dim, 0)) { }

        void setMatrix(const Int32Matrix &);
        Int32Matrix getMatrix() const
        {
            return M;
        };

        /* Get a specific value of the current matrix. */
        int32_t get(const uint32_t row, const uint32_t col) const;

        /* Set value to a specific cell of the matrix.*/
        void set(const uint32_t row, const uint32_t col, const int32_t value);
        uint32_t getDimension() const
        {
            return dim;
        }
        
        void setDimension(const uint32_t);
        int32_t getModulo() const
        {
            return n;
        };
        
        void setModulo(const int32_t);

        /* Multiply a square matrix with a column-vector of the same dimension. */
        UInt32Vector multiply(const UInt32Vector &V) const;

        Int32Matrix identity() const;

        /* Return the determinant of the current matrix.*/
        int32_t det() const;

        /* Return the inverse of the current matrix using the Gauss-Jordan method.*/
        SquareMatrix inverse() const;

    private:
        /* Find the first pivot A(n,n) != 0 if it exists. Otherwise, return dim + 1. */
        uint32_t findNonZero(const Int32Matrix &A, const uint32_t from) const;

        /* Check if the matrix is square (N x N). */
        static bool isSquare(const Int32Matrix &mat);

        /* Calculate the trace of the matrix (vlaue of tr(M)). */
        int32_t trace() const;
        int32_t trace(const Int32Matrix &A) const;

        /* Set Identity to the matrix (M_3 = [[1,0,0] ; [0,1,0] ; [0,0,1]]). */
        void setIdentity();


        void triangularize(Int32Matrix &A, Int32Matrix &I, const uint32_t k, const uint32_t lower_i, const uint32_t upper_i) const;

        // Dimension of the square matrix.
        uint32_t dim;

        // modulo from Z_n.
        int32_t n = 1;

        Int32Matrix M;
    };
}

#endif