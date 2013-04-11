
#ifndef HELLMAN_HPP
#define	HELLMAN_HPP

#include "AsymmetricCipher.hpp"

#include <vector>

class Hellman : public AsymmetricCipher
{
public:
   
   virtual Numbers encode(const Bytes_Container &);
   virtual Bytes_Container decode(const Numbers &);
   
   void setPublicKey(const Numbers &);
   Numbers getPublicKey() const;
   
   virtual void setPrivateKey(const Numbers &);
   void setModulo(const unsigned long &);
   void setCoprime(const unsigned long &);
   
private:
   Numbers public_key;
   unsigned long modulo;
   unsigned long coprime;
};

#endif	/* HELLMAN_HPP */

