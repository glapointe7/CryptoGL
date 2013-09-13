
#ifndef PSEUDORANDOMFUNCTION_HPP
#define PSEUDORANDOMFUNCTION_HPP

#include "Types.hpp"


class PseudoRandomFunction
{
protected:
   explicit PseudoRandomFunction(const uint16_t L) : output_size(L) {}
   virtual ~PseudoRandomFunction() {}
   
   virtual const UInt32Vector generate(const uint32_t value) = 0;
   virtual void initialize(const uint32_t value, const uint8_t index, UInt32Vector &A, UInt32Vector &registers) = 0;
   virtual void setKey(const BytesVector &key) = 0;
   virtual void generateSubkeys() = 0; 
   
   const uint16_t output_size;
   BytesVector key;
};

#endif