/*
 * Feistel network scheme.
 */
#ifndef FEISTEL_HPP
#define	FEISTEL_HPP

#include "BlockCipher.hpp"

template <class FeistelType, class DataType>
class Feistel : public BlockCipher<FeistelType, DataType>
{
protected:   
   typedef typename BlockCipher<FeistelType, DataType>::BytesContainer BytesContainer;
   
   /* Default Constructor : ECB mode of encryption is the default. No IV needed. */
   /*Feistel(const uint8_t round, const uint8_t block_length) 
      : BlockCipher<FeistelType, DataType>(OperationModes::ECB, block_length), rounds(round) {}
   */
   /* Constructor with no IV needed : ECB and CTR modes are accepted. */
   Feistel(const OperationModes mode, const uint8_t round, const uint8_t block_length) 
   : BlockCipher<FeistelType, DataType>(mode, block_length), rounds(round) {}
   
   /* Constructor with an IV : CBC, CFB and OFB modes are accepted. */
   //Feistel(const OperationModes mode, const uint8_t round, const uint8_t block_length, const BytesContainer &IV) 
   //: BlockCipher<FeistelType, DataType>(mode, block_length, IV), rounds(round) {}

   virtual ~Feistel() {}
   
   virtual void setKey(const BytesContainer &) = 0;
   virtual void generateSubkeys() = 0;
   virtual const DataType getIntegersFromInputBlock(const BytesContainer &) const = 0;
   virtual const DataType encodeBlock(const DataType &input) = 0;
   virtual const DataType decodeBlock(const DataType &input) = 0;
   virtual const BytesContainer getOutputBlock(const DataType &int_block) = 0;
   
   virtual FeistelType F(const FeistelType half_block, const FeistelType) const = 0;
   virtual void encodeFeistelRounds(FeistelType &L, FeistelType &R, const uint8_t) const = 0;
   virtual void decodeFeistelRounds(FeistelType &L, FeistelType &R, const uint8_t) const = 0;
   
   void setNumberOfRounds(const uint8_t rounds)
   {
      this->rounds = rounds;
   }
   
   uint8_t rounds;
};

#endif