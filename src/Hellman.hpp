
#ifndef HELLMAN_HPP
#define	HELLMAN_HPP

#include "AsymmetricCipher.hpp"
#include "exceptions/BadKey.hpp"

#include <vector>

class Hellman : public AsymmetricCipher
{      
public:   
   virtual const UInt64Container encode(const BytesContainer &clear_text) final;
   virtual const BytesContainer decode(const UInt64Container &cipher_text) final;
   
   virtual void buildPublicKey() final;
   virtual void setPrivateKey(const UInt64Container &private_key) final;
   
   void setModulo(const uint64_t &mod);
   void setDividend(const uint64_t &div);
   
private:
   using KeyNotSuperIncreasing = BadKey;
   
   uint64_t modulo;
   uint64_t dividend;
   
   void executeGlouton(std::vector<bool> &bits, const uint64_t T, const uint32_t i) const;
   bool isSuperIncresing(const UInt64Container &sequence);
};

#endif