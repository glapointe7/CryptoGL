
#ifndef IDEA_HPP
#define	IDEA_HPP

#include "BlockCipher.hpp"

class IDEA : public BlockCipher
{
public:
   explicit IDEA(const BytesContainer &key) : BlockCipher(OperationModes::ECB, 8) { setKey(key); }
   IDEA(const BytesContainer &key, const OperationModes mode) : BlockCipher(mode, 8) { setKey(key); }
   
   virtual const BytesContainer encode(const BytesContainer &clear_text) final;
   virtual const BytesContainer decode(const BytesContainer &cipher_text) final;
   
private:
   typedef std::vector<uint16_t> UInt16Container;
   
   virtual void setKey(const BytesContainer &key) final;
   virtual void generateSubkeys() final;
   virtual void generateInverseSubkeys() final;
   virtual const BytesContainer getOutputBlock(const BytesContainer &block, const bool to_encode) final;
   
   /* Decryption subkeys. */
   UInt16Container decoded_subkeys;
   UInt16Container encoded_subkeys;
};

#endif