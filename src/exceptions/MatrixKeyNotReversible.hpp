
#ifndef MATRIXKEYNOTREVERSIBLE_HPP
#define MATRIXKEYNOTREVERSIBLE_HPP

#include "BadKey.hpp"

namespace CryptoGL
{
    class MatrixKeyNotReversible : public BadKey
    {
    public:
       MatrixKeyNotReversible() {}
       explicit MatrixKeyNotReversible(const ErrorType &message) : BadKey(message) {}
    };
}

#endif