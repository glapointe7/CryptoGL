
#ifndef INTEGERSNOTCOPRIME_HPP
#define	INTEGERSNOTCOPRIME_HPP

#include "Exception.hpp"

class IntegersNotCoprime : public Exception
{
public:
   IntegersNotCoprime() {}
   IntegersNotCoprime(const std::string &text) { message = text; }
};

#endif