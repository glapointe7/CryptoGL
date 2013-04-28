
#ifndef MATRIXKEYNOTREVERSIBLE_HPP
#define	MATRIXKEYNOTREVERSIBLE_HPP

#include "BadKey.hpp"

class MatrixKeyNotReversible : public BadKey
{
public:
   MatrixKeyNotReversible() {}
   MatrixKeyNotReversible(const std::string &text) : message(text) {}
};

#endif