
#ifndef HELLMAN_HPP
#define	HELLMAN_HPP

#include "AsymmetricCipher.hpp"

#include <vector>

class Hellman : public AsymmetricCipher
{
public:
   
   const Numbers encode(const BytesContainer &) final;
   const BytesContainer decode(const Numbers &) final;
   
   void setPublicKey() final;
   void setPrivateKey(const Numbers &) final;
   
   void setModulo(const uint32_t &);
   void setCoprime(const uint32_t &);
   
private:
   uint32_t modulo;
   uint32_t coprime;
   
   void executeGlouton(std::vector<bool> &bits, const uint32_t T, const unsigned int i) const;
};

#endif