
#ifndef EMPTYALPHA_HPP
#define EMPTYALPHA_HPP

#include "BadAlpha.hpp"

class EmptyAlpha : public BadAlpha
{
public:
   EmptyAlpha() {}
   explicit EmptyAlpha(const ErrorType &message) : BadAlpha(message) {}
};

#endif