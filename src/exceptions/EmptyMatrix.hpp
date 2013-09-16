
#ifndef EMPTYMATRIX_HPP
#define EMPTYMATRIX_HPP

#include "BadMatrix.hpp"

class EmptyMatrix : public BadMatrix
{
public:
   EmptyMatrix() {}
   explicit EmptyMatrix(const ErrorType &message) : BadMatrix(message) {}
};

#endif