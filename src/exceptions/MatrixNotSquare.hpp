
#ifndef MATRIXNOTSQUARE_HPP
#define MATRIXNOTSQUARE_HPP

#include "BadMatrix.hpp"

namespace CryptoGL
{
    class MatrixNotSquare : public BadMatrix
    {
    public:
       MatrixNotSquare() {}
       explicit MatrixNotSquare(const ErrorType &message) : BadMatrix(message) {}
    };
}
    
#endif