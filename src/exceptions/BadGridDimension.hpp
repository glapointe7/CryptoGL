
#ifndef BADGRIDDIMENSION_HPP
#define BADGRIDDIMENSION_HPP

#include "Exception.hpp"

# include "../String.hpp"

class BadGridDimension : public Exception
{
public:
   BadGridDimension() {}
   BadGridDimension(const ErrorType &message, const uint8_t dim) 
   {
      const ErrorType number = String::uintToString(dim);
      ErrorType str(message);
      str.reserve(message.length() + 63 + number.length());
      str.append(" At least one of the row / col of your grid is of dimension : ");
      str.append(number);

      Exception(ErrorType(str));
   }
};

#endif