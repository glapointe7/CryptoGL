
#ifndef EMPTYKEY_HPP
#define	EMPTYKEY_HPP

#include "BadKey.hpp"

class EmptyKey : public BadKey
{
public:
   EmptyKey() {}
   EmptyKey(const std::string &text) { message = text; }
};

#endif