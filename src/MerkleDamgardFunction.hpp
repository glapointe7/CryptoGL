
#ifndef MERKLEDAMGARDFUNCTION_HPP
#define MERKLEDAMGARDFUNCTION_HPP

#include "HashFunction.hpp"

#include "Types.hpp"
#include "BigEndian.hpp"
#include "LittleEndian.hpp"
#include "Vector.hpp"

/*
 * DataType : An unsigned integer for the IV and integer input block.
 * 
 * EndianType : An endian class type (BigEndian<DataType> or LittleEndian<DataType>) that indicates
 * how to read each block to get the integer input blocks and the final hash output.
 * 
 * EndianLengthType : An endian class type (BigEndian64 or LittleEndian64) that indicates
 * how to read the initial 64-bit message length to be padded.
 * 
 * InputBlockSize : The size of each block to be processed in bytes.
 */
template <typename DataType, class EndianType, class EndianLengthType, uint8_t InputBlockSize>
class MerkleDamgardFunction : public HashFunction<DataType, EndianType>
{
static_assert(!(InputBlockSize & 0xF), "'InputBlockSize' has to be a multiple of 16.");
   
public:
   BytesVector encode(const BytesVector &message) override
   {
      const BytesVector padded_message = pad(message);
      DataTypeVector hash(this->getIV());
      
      const uint64_t padded_message_size = padded_message.size();
      for (uint64_t i = 0; i < padded_message_size; i += InputBlockSize)
      {              
         DataTypeVector int_block = this->getInputBlocks(padded_message, i);    
         compress(int_block, hash);
      }

      return this->getOutput(hash);
   }
      
protected:
   using DataTypeVector = std::vector<DataType>;
   
   MerkleDamgardFunction(const DataTypeVector &IV, const uint8_t rounds, const uint8_t output_size)
      : HashFunction<DataType, EndianType>(InputBlockSize, output_size, IV), rounds(rounds) {}
   
   virtual ~MerkleDamgardFunction() {}
   
   /* Process the main algorithm of the hash function. */
   virtual void compress(DataTypeVector &int_block, DataTypeVector &state) = 0;
   
   /* Finalize the block to get the final hashed block. */
   static void applyDaviesMayerFunction(DataTypeVector &hash, DataTypeVector &state)
   {
      const uint8_t hash_size = hash.size();
      for (uint8_t j = 0; j < hash_size; ++j)
      {
         state[j] += hash[j];
      }
   }
      
   /* Number of rounds in the compression function. */
   const uint8_t rounds;
   
   /* First byte to append when padding a message. */
   uint8_t first_byte_padding = 0x80;
   
private:
   /* Specific padding rule of the Merkel-Damgard construction for 64 and 128 bits blocks. */
   BytesVector pad(BytesVector message) const override
   {
      const uint64_t message_size = message.size();
      message.reserve(message_size + (InputBlockSize * 2));

      // Append a bit '1' to the message. Generally, it could be 0x80 or 0x01.
      message.push_back(first_byte_padding);

      // Pad with '0' bits to get a length 56 (mod 64) or 112 (mod 128) depending of the input block size. 
      const uint8_t rest = sizeof(DataType) * 2;
      const uint8_t padded_message_size = ((InputBlockSize * 2) - rest - (message.size() % InputBlockSize)) 
                                          % InputBlockSize;
      message.insert(message.end(), padded_message_size + rest - 8, 0);
      
      // Append the length of the original message in bits following the endianness of EndianLengthType.
      Vector::extend(message, EndianLengthType::toBytesVector(message_size * 8));

      return message;
   }
};

#endif