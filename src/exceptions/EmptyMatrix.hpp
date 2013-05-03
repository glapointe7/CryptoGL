
#ifndef EMPTYMATRIX_HPP
#define	EMPTYMATRIX_HPP

#include "BadMatrix.hpp"

class EmptyMatrix : public BadMatrix
{
public:
   EmptyMatrix() {}
   EmptyMatrix(const std::string &text) : BadMatrix(text) {}
};

#endif