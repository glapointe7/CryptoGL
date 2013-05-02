
#ifndef BADCHAR_HPP
#define	BADCHAR_HPP

#include "BadAlpha.hpp"

class BadChar : public BadAlpha
{
public:
   BadChar() {}
   BadChar(const std::string &text) { message = text; }
};

#endif