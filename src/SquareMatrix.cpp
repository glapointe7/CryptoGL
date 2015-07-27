#include "SquareMatrix.hpp"

#include "MathematicalTools.hpp"

#include "exceptions/EmptyMatrix.hpp"
#include "exceptions/MatrixNotSquare.hpp"
#include "exceptions/MatrixOutOfRange.hpp"

#include <algorithm>  // swap

using namespace CryptoGL;

SquareMatrix::SquareMatrix(const Int64Matrix &M, const int64_t modulo)
{
    setMatrix(M);
    this->n = modulo;
}

void SquareMatrix::setMatrix(const Int64Matrix &M)
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

bool SquareMatrix::isSquare(const Int64Matrix &mat)
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

void SquareMatrix::setDimension(const uint32_t dim)
{
    this->dim = dim;
}

void SquareMatrix::setModulo(const int64_t n)
{
    this->n = n;
}

int64_t SquareMatrix::get(const uint32_t row, const uint32_t col) const
{
    if (row >= dim || col >= dim)
    {
        throw MatrixOutOfRange("The row and/or column selected are greater or equal than the matrix dimension.");
    }

    return M[row][col];
}

Int64Vector SquareMatrix::getRow(const uint32_t row) const
{
    if (row >= dim)
    {
        throw MatrixOutOfRange("SquareMatrix (getRow): The row selected is greater or equal than the matrix dimension.");
    }
    
    return M[row];
}

void SquareMatrix::set(const uint32_t row, const uint32_t col, const int64_t value)
{
    if (row >= dim || col >= dim)
    {
        throw MatrixOutOfRange("The row and/or column selected are out of range.");
    }

    M[row][col] = value;
}

uint32_t SquareMatrix::findNonZero(const uint32_t from) const
{
    uint32_t pos = from;
    while ((M[pos][from] == 0 || !Maths::areCoprimes(M[pos][from], n)) && pos != dim)
    {
        pos++;
    }

    return pos;
}

Int64Vector SquareMatrix::multiplyByVector(const Int64Vector &V) const
{
    Int64Vector soln(dim, 0);

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

SquareMatrix SquareMatrix::multiplyBy(const SquareMatrix &A) const
{
    if(dim != A.getDimension())
    {
        throw MatrixNotSquare("The matrix 'A' does not have the same dimension as the current matrix.");
    }
    
    SquareMatrix Result(dim, n);
    for(uint32_t i = 0; i < dim; ++i)
    {
        for(uint32_t j = 0; j < dim; ++j)
        {
            uint64_t cell_result = 0;
            for(uint32_t k = 0; k < dim; ++k)
            {
                cell_result += get(i, k) * A.get(k, j);
            }
            Result.set(i, j, cell_result % n);
        }
    }
    
    return Result;
}

void SquareMatrix::setIdentity()
{
    M = Int64Matrix(dim, Int64Vector(dim, 0));
    for (uint32_t i = 0; i < dim; ++i)
    {
        M[i][i] = 1;
    }
}

Int64Matrix SquareMatrix::identity(const uint32_t dimension)
{
    Int64Matrix Mat = Int64Matrix(dimension, Int64Vector(dimension, 0));
    for (uint32_t i = 0; i < dimension; ++i)
    {
        Mat[i][i] = 1;
    }

    return Mat;
}

int64_t SquareMatrix::trace() const
{
    return trace(M);
}

int64_t SquareMatrix::trace(const Int64Matrix &A)
{
    int64_t prod = 1;
    const uint32_t A_size = A.size();
    for (uint32_t i = 0; i < A_size; ++i)
    {
        prod *= A[i][i];
    }

    return prod;
}

void SquareMatrix::swap(const uint32_t row1, const uint32_t row2)
{
    if (row2 >= dim || row2 >= dim)
    {
        throw MatrixOutOfRange("SquareMatrix (swap): The rows selected are out of range.");
    }
    
    std::swap(M[row1], M[row2]);
}

void SquareMatrix::triangularize(SquareMatrix &I, const uint32_t k,
        const uint32_t lower_i, const uint32_t upper_i)
{
    // Swap null pivot with a non null one.
    if (M[k][k] == 0 || !Maths::areCoprimes(M[k][k], n))
    {
        const uint32_t pivot = findNonZero(k);
        std::swap(M[pivot], M[k]);
        I.swap(pivot, k);
        //std::swap(A.getRow(pivot), A.getRow(k));
        //std::swap(I[pivot], I[k]);
    }

    const auto inv = Maths::getModInverse(M[k][k], n);
    for (uint32_t i = lower_i; i < upper_i; ++i)
    {
        // For rows : Li = Li + lq*Ln.
        const int64_t lq = inv * (n - M[i][k]);
        for (uint32_t j = 0; j < dim; ++j)
        {
            M[i][j] = (M[i][j] + (lq * M[k][j])) % n;
            I.set(i, j, (I.get(i, j) + (lq * I.get(k, j))) % n);
        }
    }
}

int64_t SquareMatrix::det() const
{
    int64_t determinant = 0;
    switch (dim)
    {
        // 3X3 Matrices should occur more often than 1X1 and 2X2.
        case 3:
            determinant = Maths::mod((M[0][0] * ((M[1][1] * M[2][2]) - (M[1][2] * M[2][1]))
                    - M[0][1] * ((M[1][0] * M[2][2]) - (M[1][2] * M[2][0]))
                    + M[0][2] * ((M[1][0] * M[2][1]) - (M[1][1] * M[2][0]))), n);
            break;

        // 2X2 Matrices should occur more often than 1X1.
        case 2:
            determinant = Maths::mod((M[0][0] * M[1][1]) - (M[0][1] * M[1][0]), n);
            break;

        case 1:
            determinant = Maths::mod(M[0][0], n);
            break;

        // 4X4 and higher dimensions.
        default:
        {
            SquareMatrix A = *this;
            int8_t swapping = 1;

            for (uint32_t k = 0; k < dim - 1; ++k)
            {
                // Swap the zero pivot with a non zero one.
                // If no one is found, then the column k below A(k,k) is zero.
                // Thus, det(M) = 0.
                if (A.get(k, k) == 0 || !Maths::areCoprimes(A.get(k, k), n))
                {
                    const uint64_t pivot = A.findNonZero(k);
                    if (pivot == dim)
                    {
                        return 0;
                    }
                    A.swap(pivot, k);
                    //std::swap(A.getRow(pivot), A.getRow(k));
                    swapping *= -1;
                }

                // Li = Li + q*Ln.
                const auto inv = Maths::getModInverse(A.get(k, k), n);
                for (uint32_t i = k + 1; i < dim; ++i)
                {
                    const int64_t q = (inv * (n - A.get(i, k))) % n;
                    for (uint32_t j = k + 1; j < dim; ++j)
                    {
                        A.set(i, j, (A.get(i, j) + (q * A.get(k, j))) % n);
                    }
                }
            }
            determinant = A.trace() * swapping;
        }
    }

    return determinant;
}

void SquareMatrix::inverse()
{
    // Determinant of A is positive and is in the set {0,...,mod_A-1}
    const int64_t deter = det();

    // We make sure that GCD(det(result), mod) = 1 => result is reversible in the Z_mod group.
    if (Maths::areCoprimes(deter, n))
    {
        switch (dim)
        {
            case 2:
            {
                const auto det_inv = Maths::getModInverse(deter, n);
                // Transform to a positive matrix.
                M = {
                    {static_cast<int64_t>(Maths::mod((det_inv * M[1][1]), n)), static_cast<int64_t>(Maths::mod((det_inv * -M[0][1]), n))},
                    {static_cast<int64_t>(Maths::mod((det_inv * -M[1][0]), n)), static_cast<int64_t>(Maths::mod((det_inv * M[0][0]), n))}
                };
                break;
            }
            
            case 1:
            {
                M = {
                    {static_cast<int64_t>(Maths::mod(Maths::getModInverse(deter, n), n))}
                };
                break;
            }
            
            default:
            {
                SquareMatrix I(dim, n);
                I.setIdentity();

                // Triangular inferior.
                for (uint32_t k = 0; k < dim - 1; ++k)
                {
                    triangularize(I, k, k + 1, dim);
                }

                // Triangular superior.
                for (uint32_t k = dim - 1; k >= 1; --k)
                {
                    triangularize(I, k, 0, k);
                }

                // Now we transform A to the identity matrix.
                for (uint32_t k = 0; k < dim; ++k)
                {
                    const auto inv = Maths::getModInverse(M[k][k], n);
                    for (uint32_t i = 0; i < dim; ++i)
                    {
                        I.set(k, i, Maths::mod((I.get(k, i) * inv), n));
                    }
                } // end FOR k
                M = I.getMatrix();
            } // end Default
        } // end Switch
    } // end IF
}