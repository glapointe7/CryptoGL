
#ifndef BADKEYLENGTH_HPP
#define	BADKEYLENGTH_HPP

#include "BadKey.hpp"

class BadKeyLength : public BadKey
{
public:
   BadKeyLength() {}
   BadKeyLength(const std::string &text, const uint32_t len);
};

#endif