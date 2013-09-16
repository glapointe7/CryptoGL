
#ifndef EMPTYKEY_HPP
#define EMPTYKEY_HPP

#include "BadKey.hpp"

class EmptyKey : public BadKey
{
public:
   EmptyKey() {}
   explicit EmptyKey(const ErrorType &message) : BadKey(message) {}
};

#endif