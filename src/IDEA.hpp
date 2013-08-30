
#ifndef IDEA_HPP
#define IDEA_HPP

#include "BlockCipher.hpp"
#include "BigEndian.hpp"

class IDEA : public BlockCipher<uint16_t, UInt16Vector, 8, BigEndian16>
{
public:
   explicit IDEA(const BytesVector &key) 
      : BlockCipher(OperationModes::ECB, 8, {}) { setKey(key); }
   
   IDEA(const BytesVector &key, const OperationModes mode) 
      : BlockCipher(mode, 8, {}) { setKey(key); }
   
   /* Constructor with an IV needed : Only CBC, CFB and OFB modes are accepted. */
   IDEA(const BytesVector &key, const OperationModes mode, const BytesVector &IV) 
      : BlockCipher(mode, 8, IV) { setKey(key); }
   
   /* Constructor with a vector of IV only for the mode CTR. */
   IDEA(const BytesVector &key, const IV_Vector &IV) 
      : BlockCipher(8, IV) { setKey(key); }
   
   virtual void setKey(const BytesVector &key) final;
   
private:   
   virtual void generateSubkeys() final;
   virtual void generateInverseSubkeys() final;
   virtual const UInt16Vector encodeBlock(const UInt16Vector &input) final;
   virtual const UInt16Vector decodeBlock(const UInt16Vector &input) final;
   
   /* Decryption subkeys. */
   UInt16Vector decoded_subkeys;
};

#endif