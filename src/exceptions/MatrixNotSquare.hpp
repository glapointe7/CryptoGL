
#ifndef MATRIXNOTSQUARE_HPP
#define	MATRIXNOTSQUARE_HPP

#include "BadMatrix.hpp"

class MatrixNotSquare : public BadMatrix
{
public:
   MatrixNotSquare() {}
   MatrixNotSquare(const std::string &text) : BadMatrix(text) {}
};

#endif