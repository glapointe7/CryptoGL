
#ifndef BADGRIDDIMENSION_HPP
#define BADGRIDDIMENSION_HPP

#include "Exception.hpp"

# include "../String.hpp"

class BadGridDimension : public Exception
{
public:
   BadGridDimension() {}
   explicit BadGridDimension(const ErrorType &message) : Exception(message) {}
   BadGridDimension(const ErrorType &message, const uint8_t dim)
      : Exception(message + " At least one of the row / col of your grid is of dimension : " 
         + String::uintToString(dim)) {}
};

#endif