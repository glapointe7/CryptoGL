
#ifndef MULTIPLECHAR_HPP
#define	MULTIPLECHAR_HPP

#include "BadAlpha.hpp"

class MultipleChar : public BadAlpha
{
public:
   MultipleChar();
   MultipleChar(const std::string &text) : BadAlpha(text) {}
};

#endif