
#ifndef EMPTYALPHA_HPP
#define	EMPTYALPHA_HPP

#include "BadAlpha.hpp"

class EmptyAlpha : public BadAlpha
{
public:
   EmptyAlpha() {}
   EmptyAlpha(const std::string &text) : message(text) {}
};

#endif