
#ifndef BADALPHA_HPP
#define	BADALPHA_HPP

#include "Exception.hpp"

class BadAlpha : public Exception
{
public:
   BadAlpha() {}
   BadAlpha(const std::string &text) : message(text) {}
   virtual ~BadAlpha() = 0;
};

#endif