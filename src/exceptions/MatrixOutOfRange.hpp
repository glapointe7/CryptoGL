
#ifndef MATRIXOUTOFRANGE_HPP
#define	MATRIXOUTOFRANGE_HPP

#include "BadMatrix.hpp"

class MatrixOutOfRange : public BadMatrix
{
public:
   MatrixOutOfRange() {}
   MatrixOutOfRange(const std::string &text) : BadMatrix(text) {}
};

#endif