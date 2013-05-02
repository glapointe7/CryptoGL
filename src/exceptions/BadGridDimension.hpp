
#ifndef BADGRIDDIMENSION_HPP
#define	BADGRIDDIMENSION_HPP

#include "Exception.hpp"

class BadGridDimension : public Exception
{
public:
   BadGridDimension() {}
   BadGridDimension(const std::string &text) { message = text; }
};

#endif