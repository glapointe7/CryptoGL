
#ifndef CHECKSUM_HPP
#define CHECKSUM_HPP

#include "Types.hpp"


template <class DataType>
class Checksum
{
public:
   
   virtual ~Checksum() {}
   
   virtual DataType encode(const BytesVector &) = 0;
};

#endif