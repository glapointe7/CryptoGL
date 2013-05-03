
#ifndef BADLENGTH_HPP
#define	BADLENGTH_HPP

#include "BadAlpha.hpp"

class BadLength : public BadAlpha
{
public:
   BadLength() {}
   BadLength(const std::string &text) : BadAlpha(text) {}
};

#endif