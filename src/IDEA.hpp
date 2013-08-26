
#ifndef IDEA_HPP
#define IDEA_HPP

#include "BlockCipher.hpp"
#include "BigEndian.hpp"

class IDEA : public BlockCipher<uint16_t, std::vector<uint16_t>, 8, BigEndian16 >
{
public:
   explicit IDEA(const BytesContainer &key) 
      : BlockCipher(OperationModes::ECB, 8, {}) { setKey(key); }
   
   IDEA(const BytesContainer &key, const OperationModes mode) 
      : BlockCipher(mode, 8, {}) { setKey(key); }
   
   /* Constructor with an IV needed : Only CBC, CFB and OFB modes are accepted. */
   IDEA(const BytesContainer &key, const OperationModes mode, const BytesContainer &IV) 
      : BlockCipher(mode, 8, IV) { setKey(key); }
   
   /* Constructor with a vector of IV only for the mode CTR. */
   IDEA(const BytesContainer &key, const IVContainer &IV) 
      : BlockCipher(8, IV) { setKey(key); }
   
   virtual void setKey(const BytesContainer &key) final;
   
private:   
   virtual void generateSubkeys() final;
   virtual void generateInverseSubkeys() final;
   virtual const UInt16Container encodeBlock(const UInt16Container &input) final;
   virtual const UInt16Container decodeBlock(const UInt16Container &input) final;
   
   /* Decryption subkeys. */
   UInt16Container decoded_subkeys;
};

#endif