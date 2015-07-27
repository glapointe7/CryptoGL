
#ifndef MATRIXOUTOFRANGE_HPP
#define MATRIXOUTOFRANGE_HPP

#include "BadMatrix.hpp"

namespace CryptoGL
{
    class MatrixOutOfRange : public BadMatrix
    {
    public:
       MatrixOutOfRange() {}
       explicit MatrixOutOfRange(const ErrorType &message) : BadMatrix(message) {}
    };
}

#endif