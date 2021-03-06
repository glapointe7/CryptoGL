#include "SquareMatrix.hpp"

#include "MathematicalTools.hpp"

#include "exceptions/EmptyMatrix.hpp"
#include "exceptions/MatrixNotSquare.hpp"
#include "exceptions/MatrixOutOfRange.hpp"

#include <algorithm>  // swap

using namespace CryptoGL;

void SquareMatrix::setMatrix(const Int32Matrix &M)
{
    // Exceptions : Matrix have to be square and not empty.
    if (M.empty())
    {
        throw EmptyMatrix("The matrix is empty.");
    }

    if (!isSquare(M))
    {
        throw MatrixNotSquare("The matrix has to be square.");
    }

    this->M = M;
    setDimension(M.size());
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

UInt32Vector SquareMatrix::multiply(const UInt32Vector &V) const
{
    UInt32Vector soln(dim, 0);

    uint32_t i = 0;
    for (const auto &row : M)
    {
        uint32_t j = 0;
        for (const auto &number : row)
        {
            soln[i] += number * V[j++];
        }
        soln[i++] %= n;
    }

    return soln;
}

void SquareMatrix::setIdentity()
{
    M = Int32Matrix(dim, Int32Vector(dim, 0));
    for (uint32_t i = 0; i < dim; ++i)
    {
        M[i][i] = 1;
    }
}

// Return the matrix Identity.

Int32Matrix SquareMatrix::identity() const
{
    Int32Matrix Mat = Int32Matrix(dim, Int32Vector(dim, 0));
    for (uint32_t i = 0; i < dim; ++i)
    {
        Mat[i][i] = 1;
    }

    return Mat;
}

int32_t SquareMatrix::trace() const
{
    return trace(M);
}

int32_t SquareMatrix::trace(const Int32Matrix &A) const
{
    int32_t prod = 1;
    for (uint32_t i = 0; i < dim; ++i)
    {
        prod *= A[i][i];
    }

    return prod;
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

int32_t SquareMatrix::det() const
{
    int32_t determinant = 0;

    switch (dim)
    {
        // 3X3 Matrices should occur more often than 1X1 and 2X2.
        case 3:
            determinant = (M[0][0] * ((M[1][1] * M[2][2]) - (M[1][2] * M[2][1]))
                    - M[0][1] * ((M[1][0] * M[2][2]) - (M[1][2] * M[2][0]))
                    + M[0][2] * ((M[1][0] * M[2][1]) - (M[1][1] * M[2][0]))) % n;
            break;

        // 2X2 Matrices should occur more often than 1X1.
        case 2:
            determinant = ((M[0][0] * M[1][1]) - (M[0][1] * M[1][0])) % n;
            break;

        case 1:
            determinant = M[0][0];
            break;

        default:
        {
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
            determinant = trace(A) * swapping;
        }
    }

    return determinant;
}

SquareMatrix SquareMatrix::inverse() const
{
    SquareMatrix result;
    result.setModulo(n);
    Int32Matrix A;

    // Determinant of A is positive and is in the set {0,...,mod_A-1}
    const int32_t deter = det();

    // We make sure that GCD(det(result), mod) = 1 => result is reversible in the Z_mod group.
    if (Maths::areCoprimes(deter, n))
    {
        switch (dim)
        {
            case 1:
            {
                A = {
                    {(Maths::getModInverse(deter, n) + n) % n}
                };
                result.setMatrix(A);
                break;
            }

            case 2:
            {
                const int32_t det_inv = Maths::getModInverse(deter, n);
                // Transform to a positive matrix.
                A = {
                    {(((det_inv * M[1][1]) % n) + n) % n, (((det_inv * -M[0][1]) % n) + n) % n},
                    {(((det_inv * -M[1][0]) % n) + n) % n, (((det_inv * M[0][0]) % n) + n) % n}
                };
                result.setMatrix(A);
                break;
            }

            default:
            {
                A = M;
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
                result.setMatrix(I);
            } // end Default
        } // end Switch
    } // end IF

    return result;
}