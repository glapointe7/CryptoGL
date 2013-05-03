
#ifndef HELLMAN_HPP
#define	HELLMAN_HPP

#include "AsymmetricCipher.hpp"
#include "exceptions/BadKey.hpp"

#include <vector>

class Hellman : public AsymmetricCipher
{   
   using KeyNotSuperIncreasing = BadKey;
   
public:   
   const UInt64Container encode(const BytesContainer &) final;
   const BytesContainer decode(const UInt64Container &) final;
   
   void setPublicKey() final;
   void setPrivateKey(const UInt64Container &) final;
   
   void setModulo(const uint64_t &mod);
   void setDividend(const uint64_t &div);
   
private:
   uint64_t modulo;
   uint64_t dividend;
   
   void executeGlouton(std::vector<bool> &bits, const uint64_t T, const uint32_t i) const;
};

#endif