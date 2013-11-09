
#ifndef LRC_HPP
#define LRC_HPP

#include "Checksum.hpp"

class LRC : public Checksum <uint8_t>
{
public:
   LRC() {}
   
   virtual uint8_t encode(const BytesVector &input) final;
};

#endif