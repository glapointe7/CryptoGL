
#ifndef BLOCKCIPHER_HPP
#define BLOCKCIPHER_HPP

#include "SymmetricCipher.hpp"

#include "BlockCipherOperationModes.hpp"
#include "BlockCipherStrategy.hpp"
#include "exceptions/BadKeyLength.hpp"

class BlockCipher : public SymmetricCipher
{
   using BadIVLength = BadKeyLength;
   
public:
   typedef std::vector<uint32_t> UInt32Container;
   typedef std::vector<uint64_t> UInt64Container;
   
   BlockCipher(const OperationModes modes);
   virtual ~BlockCipher();
   
   virtual const BytesContainer encode(const BytesContainer &) = 0;
   virtual const BytesContainer decode(const BytesContainer &) = 0;
   
   virtual void setKey(const BytesContainer &) = 0;
   
protected:
   virtual const UInt64Container getKeySchedule() = 0;

   BlockCipherStrategy *blockStrategy;
};

#endif

