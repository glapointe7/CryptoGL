/*
 * Symmetric cipher abstract class for modern cryptology.
 */
#ifndef SYMMETRICCIPHER_HPP
#define SYMMETRICCIPHER_HPP

#include "Cipher.hpp"

#include <string>
#include <vector>

class SymmetricCipher : public Cipher<std::vector<uint8_t> >
{
public:
   typedef std::vector<uint8_t> BytesContainer;
   typedef std::vector<uint16_t> UInt16Container;
   typedef std::vector<uint32_t> UInt32Container;
   typedef std::vector<uint64_t> UInt64Container;

protected:
   virtual ~SymmetricCipher() {}

   virtual const BytesContainer encode(const BytesContainer &) = 0;
   virtual const BytesContainer decode(const BytesContainer &) = 0;
   
   /* Check the key provided by the user and set it if correct. */
   virtual void setKey(const BytesContainer &) = 0;
   
   BytesContainer key;
};

#endif