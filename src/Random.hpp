
#ifndef RANDOM_HPP
#define	RANDOM_HPP

#include <vector>
#include <string>

class Random
{
public:
   virtual ~Random() {}
   
   static const std::vector<uint8_t> generateBytesVector(const uint32_t length);
   static const std::string generateString(const uint32_t length, const std::string &alphabet);
};

#endif