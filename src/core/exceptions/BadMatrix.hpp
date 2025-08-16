
#ifndef BADMATRIX_HPP
#define BADMATRIX_HPP

#include "Exception.hpp"

class BadMatrix : public Exception
{
public:
   BadMatrix() {}
   explicit BadMatrix(const ErrorType &message) : Exception(message) {}
   virtual ~BadMatrix() {}
};

#endif