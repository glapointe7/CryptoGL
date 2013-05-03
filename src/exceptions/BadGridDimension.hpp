
#ifndef BADGRIDDIMENSION_HPP
#define	BADGRIDDIMENSION_HPP

#include "Exception.hpp"

#include <sstream>

class BadGridDimension : public Exception
{
public:
   BadGridDimension() {}
   BadGridDimension(const std::string &text, const uint8_t dim) 
   {
      std::stringstream ss;
      ss << text
              << " At least one of the row / col of your grid is of dimension : "
              << dim;

      Exception(ss.str());
   }
};

#endif