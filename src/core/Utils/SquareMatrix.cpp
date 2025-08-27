#include "SquareMatrix.hpp"
#include "MathematicalTools.hpp"
#include "../exceptions/EmptyMatrix.hpp"
#include "../exceptions/MatrixNotSquare.hpp"
#include "../exceptions/MatrixOutOfRange.hpp"
#include <algorithm>  // swap

using namespace CryptoGL;


SquareMatrix::SquareMatrix(const Int32Matrix& M, const int32_t modulo) {
    setMatrix(M);
    setModulo(modulo);
}

SquareMatrix::SquareMatrix(Int32Matrix&& M, const int32_t modulo) {
    setMatrix(M);
    setModulo(modulo);
}

void SquareMatrix::setMatrix(const Int32Matrix& M) {
    if (M.empty()) [[unlikely]] {
        throw EmptyMatrix("The matrix is empty.");
    }
    if (!isSquare(M)) [[unlikely]] {
        throw MatrixNotSquare("The matrix has to be square.");
    }
    this->M = M;                                
    setDimension(M.size());
}

void SquareMatrix::setMatrix(Int32Matrix&& M) {   
    if (M.empty()) [[unlikely]] {
        throw EmptyMatrix("The matrix is empty.");
    }
    if (!isSquare(M)) [[unlikely]] {
        throw MatrixNotSquare("The matrix has to be square.");
    }
    this->M = std::move(M);           // Move assignment - ZERO COPY!
    setDimension(this->M.size());
}

void SquareMatrix::setDimension(const uint32_t dim)
{
    this->dim = dim;
}

void SquareMatrix::setModulo(const int32_t n)
{
    this->n = n;
}

int32_t SquareMatrix::get(const uint32_t row, const uint32_t col) const
{
    if (row >= dim || col >= dim)
    {
        throw MatrixOutOfRange("The row and/or column selected are greater or equal than the matrix dimension.");
    }

    return M[row][col];
}

void SquareMatrix::set(const uint32_t row, const uint32_t col, const int32_t value)
{
    if (row >= dim || col >= dim)
    {
        throw MatrixOutOfRange("The row and/or column selected are out of range.");
    }

    M[row][col] = value;
}

uint32_t SquareMatrix::findNonZero(const Int32Matrix &A, const uint32_t from) const
{
    uint32_t pos = from;
    while ((A[pos][from] == 0 || !Maths::areCoprimes(A[pos][from], n)) && pos != dim)
    {
        pos++;
    }

    return pos;
}

UInt32Vector SquareMatrix::multiply(const UInt32Vector &V) const noexcept
{
    UInt32Vector result(dim, 0);
    
    // Cache-friendly row-major access with explicit indexing
    for (uint32_t i = 0; i < dim; ++i) {
        uint64_t sum = 0;                    // Use wider type to avoid overflow
        const auto& row = M[i];              // Cache row reference
        
        // Inner loop - potential for auto-vectorization
        for (uint32_t j = 0; j < dim; ++j) {
            sum += static_cast<uint64_t>(row[j]) * V[j];
        }
        result[i] = static_cast<uint32_t>(sum % n);
    }
    
    return result;
}

void SquareMatrix::setIdentity() noexcept
{
     M.assign(dim, Int32Vector(dim, 0));    
    
    // Vectorizable loop for setting diagonal
    for (uint32_t i = 0; i < dim; ++i) {
        M[i][i] = 1;
    }
}

// Return the matrix Identity.
Int32Matrix SquareMatrix::identity() const noexcept
{
    Int32Matrix mat;
    mat.reserve(dim);                         // Pre-allocate to avoid reallocations
    
    for (uint32_t i = 0; i < dim; ++i) {
        mat.emplace_back(dim, 0);             // Construct in-place
        mat[i][i] = 1;                         // Set diagonal element
    }
    
    return mat;
}

int32_t SquareMatrix::trace() const noexcept
{
    return trace(M);
}

int32_t SquareMatrix::trace(const Int32Matrix &A) const noexcept
{
    int32_t sum = 0;
    for (uint32_t i = 0; i < dim; ++i)
    {
        sum += A[i][i];
    }

    return sum % n;
}

bool SquareMatrix::isSquare(const Int32Matrix &mat)
{
    const uint32_t A_size = mat.size();
    for (const auto &V : mat)
    {
        if (V.size() != A_size)
        {
            return false;
        }
    }

    return true;
}

void SquareMatrix::triangularize(Int32Matrix &A, Int32Matrix &I, const uint32_t k,
        const uint32_t lower_i, const uint32_t upper_i) const
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
        const int32_t lq = (inv * (n - A[i][k]));
        for (uint32_t j = 0; j < dim; ++j)
        {
            A[i][j] = (A[i][j] + (lq * A[k][j])) % n;
            I[i][j] = (I[i][j] + (lq * I[k][j])) % n;
        }
    }
}

int32_t SquareMatrix::diagonal_product() const noexcept
{
    return diagonal_product(M);
}

int32_t SquareMatrix::diagonal_product(const Int32Matrix &A) const noexcept
{
    int32_t result = 0;
    for (uint32_t i = 0; i < dim; ++i)
    {
        result *= A[i][i];
    }

    return result % n;
}

int32_t SquareMatrix::det() const noexcept {
    switch (dim) {
        case 1:
            return Maths::Mod(M[0][0], n);
            
        case 2:
            return Maths::Mod(
                ((M[0][0] * M[1][1]) -
                (M[0][1] * M[1][0])), n
            );
                 
        case 3: {
            // Optimized 3x3 determinant using Sarrus rule with overflow protection
            const int32_t positive = 
                (M[0][0] * M[1][1] * M[2][2]) +
                (M[0][1] * M[1][2] * M[2][0]) +
                (M[0][2] * M[1][0] * M[2][1]);
                
            const int32_t negative = 
                (M[0][2] * M[1][1] * M[2][0]) +
                (M[0][1] * M[1][0] * M[2][2]) +
                (M[0][0] * M[1][2] * M[2][1]);
                
            return Maths::Mod((positive - negative), n);
        }
        
        default:
            return det_gaussian();       // Optimized Gaussian elimination
    }
}

int32_t SquareMatrix::det_gaussian() const noexcept
{
    int32_t determinant = 0;

    Int32Matrix A = M;
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
    determinant = diagonal_product(A) * swapping;

    return determinant;
}

std::optional<SquareMatrix> SquareMatrix::inverse() const
{
    // Determinant of A is positive and is in the set {0,...,mod_A-1}
    const int32_t deter = det();
    if (!Maths::areCoprimes(deter, n)) {
        return std::nullopt;
    }

    // We make sure that GCD(det(result), mod) = 1 => result is reversible in the Z_mod group.
    switch (dim)
    {
        case 1:
        {
            Int32Matrix A = {{(Maths::getModInverse(deter, n) + n) % n}};
            return SquareMatrix(std::move(A), n);
        }

        case 2:
        {
            const int32_t det_inv = Maths::getModInverse(deter, n);
            // Transform to a positive matrix.
            Int32Matrix A = {
                {(((det_inv * M[1][1]) % n) + n) % n, (((det_inv * -M[0][1]) % n) + n) % n},
                {(((det_inv * -M[1][0]) % n) + n) % n, (((det_inv * M[0][0]) % n) + n) % n}
            };
            return SquareMatrix(std::move(A), n);
        }

        default:
        {
            Int32Matrix A = M;
            Int32Matrix I = identity();

            // Triangular inferior.
            for (uint32_t k = 0; k < dim - 1; ++k)
            {
                triangularize(A, I, k, k + 1, dim);
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
            return SquareMatrix(std::move(I), n);
        } // end Default
    } // end Switch
}