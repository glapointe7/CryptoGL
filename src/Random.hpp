
#ifndef RANDOM_HPP
#define	RANDOM_HPP

#include <vector>
#include <string>

class Random
{
public:
   virtual ~Random() {}
   
   /* Generate random bytes vector. */
   static const std::vector<uint8_t> generateBytesVector(const uint32_t length);
   
   /* Generate random string. */
   static const std::string generateString(const uint32_t length, const std::string &alphabet);
   
   /* Generate unique and consecutive integers starting at zero for row/column permutation vector. */
   static const std::vector<int32_t> generatePermutationVector(const uint32_t length);
};

#endif