
#ifndef BADCHAR_HPP
#define	BADCHAR_HPP

#include "BadAlpha.hpp"

class BadChar : public BadAlpha
{
public:

   BadChar() {}

   BadChar(const std::string &text, const char c)
      : BadAlpha(text + " The bad character is : '" + c + "'.") {}
};

#endif