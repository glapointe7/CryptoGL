/*
 * Symmetric cipher abstract class for modern cryptology.
 */
#ifndef SYMMETRICCIPHER_HPP
#define	SYMMETRICCIPHER_HPP

#include "Cipher.hpp"

#include <string>
#include <vector>

class SymmetricCipher : public Cipher<std::vector<uint8_t> >
{
public:
   typedef std::vector<uint8_t> BytesContainer;
   
   virtual ~SymmetricCipher() {}

   virtual const BytesContainer encode(const BytesContainer &) = 0;
   virtual const BytesContainer decode(const BytesContainer &) = 0;

   static void save(const std::string &filename, const BytesContainer &data);
   static const BytesContainer load(const std::string &filename);

protected:   
   BytesContainer key;
};

#endif