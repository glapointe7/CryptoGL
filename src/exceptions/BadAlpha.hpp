
#ifndef BADALPHA_HPP
#define BADALPHA_HPP

#include "Exception.hpp"

class BadAlpha : public Exception
{
public:
   BadAlpha() {}
   explicit BadAlpha(const ErrorType &text) : Exception(text) {}
   virtual ~BadAlpha() {};
};

#endif