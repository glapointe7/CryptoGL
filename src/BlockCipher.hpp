#ifndef BLOCKCIPHER_HPP
#define	BLOCKCIPHER_HPP

#include "SymmetricCipher.hpp"

class BlockCipher : public SymmetricCipher
{
public:
   typedef std::vector<uint32_t> UInt32Container;
   typedef std::vector<uint64_t> UInt64Container;
   
   virtual ~BlockCipher() {}
   
   virtual const BytesContainer encode(const BytesContainer &) = 0;
   virtual const BytesContainer decode(const BytesContainer &) = 0;
   
   virtual void setKey(const BytesContainer &) = 0;
   virtual UInt64Container getKeySchedule() = 0;
   
protected:

};

#endif

