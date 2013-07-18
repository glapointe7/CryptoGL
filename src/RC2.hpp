/*
 * Source : http://www.ietf.org/rfc/rfc2268.txt
 */
#ifndef RC2_HPP
#define	RC2_HPP

#include "BlockCipher.hpp"

class RC2 : public BlockCipher
{
public:
   explicit RC2(const BytesContainer &key) : BlockCipher(OperationModes::ECB, 8) { setKey(key); }
   RC2(const BytesContainer &key, const OperationModes mode) : BlockCipher(mode, 8) { setKey(key); }
   
   virtual const BytesContainer encode(const BytesContainer &clear_text) final;
   virtual const BytesContainer decode(const BytesContainer &cipher_text) final;
     
private:
   virtual void setKey(const BytesContainer &key) final;
   virtual void generateSubkeys() final;
   virtual const BytesContainer getOutputBlock(const BytesContainer &block, const bool to_encode) final;
   
   void mixUp(uint16_t *input, const uint8_t index, const uint8_t key_index) const;
   void mash(uint16_t *input, const uint8_t index) const;
   
   void inverseMixUp(uint16_t *input, const uint8_t index, const uint8_t key_index) const;
   void inverseMash(uint16_t *input, const uint8_t index) const;
   
   static const uint8_t pi_table[256];
   static const uint8_t mixup_rotation[4];
};

#endif