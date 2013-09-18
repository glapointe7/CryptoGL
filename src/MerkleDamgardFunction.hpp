
#ifndef MERKLEDAMGARDFUNCTION_HPP
#define MERKLEDAMGARDFUNCTION_HPP

#include "HashFunction.hpp"

#include "Types.hpp"
#include "BigEndian.hpp"
#include "LittleEndian.hpp"

template <typename DataType, class EndianType, class EndianLengthType, uint8_t InputBlockSize>
class MerkleDamgardFunction : public HashFunction<DataType, EndianType>
{
static_assert(!(InputBlockSize & 0xF), "'InputBlockSize' has to be a multiple of 16.");
static_assert(std::is_integral<DataType>::value, "Type 'DataType' must be an integral type.");
   
public:
   const BytesVector encode(const BytesVector &message)
   {
      const BytesVector padded_message = pad(message);
      DataTypeVector hash(this->IV);
      
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
      
   /* Number of rounds in the compression function. */
   const uint8_t rounds;
   
   /* First byte to append when padding a message. */
   uint8_t first_byte_padding = 0x80;
   
private:
   virtual const BytesVector pad(const BytesVector &message) const final
   {
      const uint64_t message_size = message.size();
      BytesVector padded_message(message);
      padded_message.reserve(message_size + (InputBlockSize << 1));

      // Append a bit '1' to the message. Generally, it could be 0x80 or 0x01.
      padded_message.push_back(first_byte_padding);

      // Pad with '0' bits enough
      const uint8_t rest = sizeof(DataType) << 1;
      const uint8_t padded_message_size = ((InputBlockSize << 1) - rest - (padded_message.size() & (InputBlockSize - 1))) 
                                          & (InputBlockSize - 1);
      padded_message.insert(padded_message.end(), padded_message_size + rest - 8, 0);
      
      // Append the length of the original message in bits following the endianness of EndianLengthType.
      const BytesVector endian_message_size = EndianLengthType::toBytesVector(message_size << 3);
      padded_message.insert(padded_message.end(), endian_message_size.begin(), endian_message_size.end());

      return padded_message;
   }
};

#endif