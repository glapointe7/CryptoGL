/*
 * Feistel network scheme.
 */
#ifndef FEISTEL_HPP
#define FEISTEL_HPP

#include "BlockCipher.hpp"

template <class FeistelType, class DataType, uint8_t InputBlockSize, class IndianType, class SubkeyType = FeistelType>
class Feistel : public BlockCipher<SubkeyType, DataType, InputBlockSize, IndianType>
{
protected:      
   /* Default Constructor : ECB mode of encryption is the default. No IV needed. */
   /*Feistel(const uint8_t round) 
      : BlockCipher<SubkeyType, DataType, InputBlockSize, IndianType>(OperationModes::ECB, round, {}) {}
   */
   /* Constructor with no IV needed : ECB and CTR modes are accepted. */
   Feistel(const OperationModes mode, const uint8_t round) 
      : BlockCipher<SubkeyType, DataType, InputBlockSize, IndianType>(mode, round, {}) {}
   
   /* Constructor with an IV : CBC, CFB and OFB modes are accepted. */
   Feistel(const OperationModes mode, const uint8_t round, const BytesVector &IV) 
      : BlockCipher<SubkeyType, DataType, InputBlockSize, IndianType>(mode, round, IV) {}
   
   /* Constructor with a vector of IV only for the mode CTR. */
   Feistel(const uint8_t round, const IV_Vector &IV) 
      : BlockCipher<SubkeyType, DataType, InputBlockSize, IndianType>(round, IV) {}

   virtual ~Feistel() {}
   
   virtual void setKey(const BytesVector &) = 0;
   virtual void generateSubkeys() = 0;
   virtual const DataType encodeBlock(const DataType &input) = 0;
   virtual const DataType decodeBlock(const DataType &input) = 0;
   
   virtual const FeistelType F(const FeistelType half_block, const uint8_t) const = 0;
   virtual void encodeFeistelRounds(FeistelType &L, FeistelType &R, const uint8_t) const = 0;
   virtual void decodeFeistelRounds(FeistelType &L, FeistelType &R, const uint8_t) const = 0;
};

#endif