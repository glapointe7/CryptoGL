
#ifndef KEYNOTSUPERINCREASING_HPP
#define	KEYNOTSUPERINCREASING_HPP

#include "BadKey.hpp"

class KeyNotSuperIncreasing : BadKey
{
public:
   KeyNotSuperIncreasing() {}
   KeyNotSuperIncreasing(const std::string &text) { message = text; }
};

#endif