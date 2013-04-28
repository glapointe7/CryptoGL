
#ifndef BADKEY_HPP
#define	BADKEY_HPP

#include "Exception.hpp"

class BadKey : public Exception
{
public:
   BadKey() {}
   BadKey(const std::string &text) : message(text) {}
   virtual ~BadKey() = 0;
};

#endif