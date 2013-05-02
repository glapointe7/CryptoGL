
#ifndef BADMODULO_HPP
#define	BADMODULO_HPP

#include "Exception.hpp"

class BadModulo : public Exception
{
public:
   BadModulo() {}
   BadModulo(const std::string &text) { message = text; }
};

#endif