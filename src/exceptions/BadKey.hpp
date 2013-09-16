
#ifndef BADKEY_HPP
#define BADKEY_HPP

#include "Exception.hpp"

class BadKey : public Exception
{
public:
   BadKey() {}
   explicit BadKey(const ErrorType &message) : Exception(message) {}
   virtual ~BadKey() {};
};

#endif