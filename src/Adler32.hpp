/*
 * Source : tools.ietf.org/html/rfc1950
 */
#ifndef ADLER32_HPP
#define ADLER32_HPP

#include "Checksum.hpp"

class Adler32 : public Checksum<uint32_t>
{
public:
   Adler32() {}
   
   virtual uint32_t encode(const BytesVector &input) final;
   
private:
   /* largest prime smaller than 65536. */
   static constexpr uint16_t mod_prime = 65521;
};

#endif