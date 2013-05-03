
#ifndef BADALPHA_HPP
#define	BADALPHA_HPP

#include "Exception.hpp"

class BadAlpha : public Exception
{
public:
   BadAlpha() {}
   BadAlpha(const std::string &text) : Exception(text) {}
   virtual ~BadAlpha() {};
};

#endif