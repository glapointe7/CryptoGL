
#ifndef IDEA_HPP
#define IDEA_HPP

#include "BlockCipher.hpp"
#include "Endian.hpp"

class IDEA : public BlockCipher<uint16_t, UInt16Vector, 8, BigEndian16>
{
public:
   /* Constructor with an IV needed : Only CBC, CFB and OFB modes are accepted. */
   IDEA(const BytesVector &key, const OperationModes mode, const BytesVector &IV) 
      : BlockCipher(mode, 8, IV) { setKey(key); }
      
   explicit IDEA(const BytesVector &key) 
      : IDEA(key, OperationModes::ECB, {}) {}
   
   IDEA(const BytesVector &key, const OperationModes mode) 
      : IDEA(key, mode, {}) {}
   
   void setKey(const BytesVector &key) override;
   
private:   
   void generateSubkeys() override;
   void generateInverseSubkeys() override;
   UInt16Vector encodeBlock(const UInt16Vector &input) override;
   UInt16Vector decodeBlock(const UInt16Vector &input) override;
   
   /* Decryption subkeys. */
   UInt16Vector decoded_subkeys;
};

#endif