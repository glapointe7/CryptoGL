
#ifndef PSEUDORANDOMFUNCTION_HPP
#define PSEUDORANDOMFUNCTION_HPP

#include "Types.hpp"

template <class SeedType, class GenerateType>
class PseudoRandomFunction
{
protected:
   PseudoRandomFunction() {}
   explicit PseudoRandomFunction(const SeedType seed) : seed(seed) {}
   virtual ~PseudoRandomFunction() {}
   
   virtual const GenerateType generate() = 0;
   
   SeedType seed;
};

#endif