/*
 * Square matrix in M_k(Z_n).
 */
#pragma once

#include "../types/Types.hpp"
#include <optional>

namespace CryptoGL
{
    class SquareMatrix
    {
    public:
        SquareMatrix() noexcept = default;
        SquareMatrix(const uint32_t dim, const int32_t modulo) noexcept
            : dim(dim), n(modulo), M(dim, Int32Vector(dim, 0)) { }

        SquareMatrix(SquareMatrix&) noexcept = default;
        SquareMatrix(SquareMatrix&&) noexcept = default;

        SquareMatrix(const Int32Matrix&, const int32_t modulo);
        SquareMatrix(Int32Matrix&&, const int32_t modulo);

        SquareMatrix& operator=(const SquareMatrix&) = default;
        SquareMatrix& operator=(SquareMatrix&&) = default;

        void setMatrix(const Int32Matrix &M);
        void setMatrix(Int32Matrix&& M);
        
        [[nodiscard]] Int32Matrix getMatrix() const noexcept
        {
            return M;
        };

        /* Get a specific value of the current matrix. */
        [[nodiscard]] int32_t get(const uint32_t row, const uint32_t col) const;

        /* Set value to a specific cell of the matrix.*/
        void set(const uint32_t row, const uint32_t col, const int32_t value);
        
        [[nodiscard]] uint32_t getDimension() const noexcept
        {
            return dim;
        }
        
        void setDimension(const uint32_t);
        [[nodiscard]] int32_t getModulo() const noexcept
        {
            return n;
        };
        
        void setModulo(const int32_t);

        /* Multiply a square matrix with a column-vector of the same dimension. */
        [[nodiscard]] UInt32Vector multiply(const UInt32Vector& V) const noexcept;

        [[nodiscard]] Int32Matrix identity() const noexcept;

        /* Return the determinant of the current matrix.*/
        [[nodiscard]] int32_t det() const noexcept;
        [[nodiscard]] int32_t det_gaussian() const noexcept;

        /* Return the inverse of the current matrix using the Gauss-Jordan method.*/
        [[nodiscard]] std::optional<SquareMatrix> inverse() const;

    private:
        /* Find the first pivot A(n,n) != 0 if it exists. Otherwise, return dim + 1. */
        [[nodiscard]] uint32_t findNonZero(const Int32Matrix &A, const uint32_t from) const;

        /* Check if the matrix is square (N x N). */
        [[nodiscard]] static bool isSquare(const Int32Matrix &mat);

        /* Calculate the trace of the matrix (vlaue of tr(M)). */
        [[nodiscard]] int32_t trace() const noexcept;
        [[nodiscard]] int32_t trace(const Int32Matrix &A) const noexcept;

        [[nodiscard]] int32_t diagonal_product() const noexcept;
        [[nodiscard]] int32_t diagonal_product(const Int32Matrix &A) const noexcept;

        /* Set Identity to the matrix (M_3 = [[1,0,0] ; [0,1,0] ; [0,0,1]]). */
        void setIdentity() noexcept;


        void triangularize(Int32Matrix &A, Int32Matrix &I, const uint32_t k, const uint32_t lower_i, const uint32_t upper_i) const;

        // Dimension of the square matrix.
        uint32_t dim;

        // modulo from Z_n.
        int32_t n = 1;

        Int32Matrix M;
    };
}