
#ifndef BADMATRIX_HPP
#define	BADMATRIX_HPP

#include "Exception.hpp"

class BadMatrix : public Exception
{
public:
   BadMatrix() {}
   BadMatrix(const std::string &text) : Exception(text) {}
   virtual ~BadMatrix() {}
};

#endif