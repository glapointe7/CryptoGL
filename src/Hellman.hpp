
#ifndef HELLMAN_HPP
#define	HELLMAN_HPP

#include "AsymmetricCipher.hpp"

#include <vector>

class Hellman : public AsymmetricCipher
{
public:
   
   virtual const Numbers encode(const BytesContainer &);
   virtual const BytesContainer decode(const Numbers &);
   
   virtual void setPublicKey();
   virtual void setPrivateKey(const Numbers &);
   
   void setModulo(const unsigned long &);
   void setCoprime(const unsigned long &);
   
private:
   unsigned long modulo;
   unsigned long coprime;
   
   void executeGlouton(std::vector<bool> &bits, const unsigned long T, const unsigned int i) const;
};

#endif